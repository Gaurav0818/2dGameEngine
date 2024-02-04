#ifndef ECS_h
#define ECS_h

#include <vector>
#include <bitset>
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>

constexpr unsigned int MAX_COMPONENTS = 32;

/**
 * @brief
 * A bitset used to keep track of which components an entity has,
 * and also helps keep track of which entities a system is interested in.
 */
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent
{
protected:
	inline static int s_nextId = 0;
};

/**
 * @brief
 * Used to assign a unique ID to a component type
 */
template <typename T>
class Component: public IComponent
{
	friend class System; // System can access GetId()

private:
	// Returns the unique ID for this component type
	static int GetId()
	{
		static auto id = s_nextId++;
		return id;
	}
};

/**
 * @brief
 * An entity is a unique identifier for a game object.
 */
class Entity
{
public:
	Entity(int id) : m_id(id) {}
	Entity(const Entity& other) = default;

	~Entity() = default;

	int GetId() const { return m_id; }
	//...

	bool operator == (const Entity& other) const { return m_id == other.m_id; }
	bool operator != (const Entity& other) const { return m_id != other.m_id; }
	Entity& operator= (const Entity& other) = default;
	bool operator >(const Entity& other) const { return m_id > other.m_id; }
	bool operator <(const Entity& other) const { return m_id < other.m_id; }

private:
	int m_id;
};


/**
 * @brief
 * The System processes the entities that have the components it requires.
 */
class System
{
public:
	System() = default;
	~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityToSystem(Entity entity);
	std::vector<Entity> GetSystemEntities() const { return m_Entities; }
		
	const Signature& GetComponentSignature() const { return m_ComponentSignature; }

	// Define the components that the system requires
	template <typename TComponent>
	inline void RequireComponent()
	{
		const auto componentId = Component<TComponent>::GetId();
		m_ComponentSignature.set(componentId);
	}
	
private:
	Signature m_ComponentSignature;
	std::vector<Entity> m_Entities;

};

class IPool
{
public:
	virtual ~IPool() = 0;
};

template <typename T>
class Pool : public IPool
{
public:
	Pool (int size = 100)
	{
		m_data.reserve(size);
	}
	
	~Pool() override = default;

	
	bool isEmpty() const { return m_data.empty(); }
	int Getsize() const { return m_data.size(); }
	void Resize(int n) { m_data.resize(n); }
	void Clear() { m_data.clear(); }
	void Add(T component) { m_data.push_back(component); }
	void Set(int index, T object) { m_data[index] = object; }
	void Remove(int index) { m_data.erase(m_data.begin() + index); }
	void Remove(T& component)
	{
		m_data.erase(std::remove(m_data.begin(), m_data.end(), component), m_data.end());
	}
	T& Get(int index) { return static_cast<T&>(m_data[index]); }
	T& operator[] (int index) { return static_cast<T&>(m_data[index]); }
	
private:
	std::vector<T> m_data;
};

/**
 * @brief
 * The Registry is used to create and manage entities, add System and their components.
 */
class Registry
{
public:
	Registry() = default;
	~Registry() = default;
	void Update();

	// Entity Management
	Entity CreateEntity();

	// Component Management
	template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
	template <typename TComponent> void RemoveComponent(Entity entity);
	template <typename TComponent> bool HasComponent(Entity entity);

	// System Management
	template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
	template <typename TSystem> void RemoveSystem();
	template <typename TSystem> bool HasSystem() const;
	template <typename TSystem> TSystem& GetSystem() const;

	/**
	 * @brief
	 * Checks the components signature of an entity and add the entity to the systems
	 * that are interested in it.
	 */
	void AddEntityToSystems(Entity entity) const;

	// TODO:
	// KillEntity()
	// GetComponent(Entity entity)
	//
	// AddSystem()
	// RemoveSystem()
	// HasSystem()
	// GetSystem()

private:
	int m_numEntities = 0;

	/** 
	 * @brief 
	 * Vector of Component Pools,
	 * \n each pool stores the components of a specific type
	 * \n [Vector index = component ID]
	 * \n [Pool index = entity ID]
	 */
	std::vector<IPool*> m_componentPools;

	/**
	 * @brief
	 * Vector of Component Signatures per Entity,
	 * \n The signature lets us know which components are turned "on" for each entity.
	 * \n [Vector index = entity ID]
	 */
	std::vector<Signature> m_entityComponentSignatures;

	/**
	 * @brief
	 * Map of Active Systems,
	 * \n [Key = System TypeID]
	 * \n [Value = System]
	 */
	std::unordered_map<std::type_index, System*> m_systems;

	/**
	 * @brief
	 * Set of Entities to be Added,
	 * \n Entities are added at the end of the frame.
	 */
	std::set<Entity> m_entitiesToBeAdded;
	
	/**
	 * @brief
	 *	Set of Entities To be Deleted/Removed from the Registry,
	 *	\n Entities are removed at the end of the frame.
	 */
	std::set<Entity> m_entitiesToBeKilled;
};

template <typename TComponent, typename ... TArgs>
void Registry::AddComponent(Entity entity, TArgs&&... args)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	if(componentId >= m_componentPools.size())
		m_componentPools.resize(componentId + 1, nullptr);

	if(m_componentPools[componentId] == nullptr)
		m_componentPools[componentId] = new Pool<TComponent>();
	
	Pool<TComponent>* componentPool = m_componentPools[componentId];
	
	if(entityId >= componentPool->Getsize())
		componentPool->Resize(entityId + 1);
	
	TComponent newComponent(std::forward<TArgs>(args)...);

	componentPool->Set(entityId, newComponent);
	m_entityComponentSignatures[entityId].set(componentId);
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	m_entityComponentSignatures[entityId].set(componentId, false);
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity)
{
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	return m_entityComponentSignatures[entityId].test(componentId);

}
template <typename TSystem, typename ... TArgs>
void Registry::AddSystem(TArgs&&... args)
{
	TSystem* newSystem(new TSystem(std::forward<TArgs>(args)...));
	m_systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <typename TSystem>
void Registry::RemoveSystem()
{
	auto system = m_systems.find(std::type_index(typeid(TSystem)));
	m_systems.erase(system);
}

template <typename TSystem>
bool Registry::HasSystem() const
{
	return  m_systems.contains(std::type_index(typeid(TSystem)));
}

template <typename TSystem>
TSystem& Registry::GetSystem() const
{
	auto system = m_systems.find(std::type_index(typeid(TSystem)));
	return *(std::static_pointer_cast<TSystem>(system->second));
}

#endif
