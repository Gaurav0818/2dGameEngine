#ifndef ECS_h
#define ECS_h

#include <vector>
#include <bitset>

const unsigned int MAX_COMPONENTS = 32;
///<summary>
/// We use bitset (1s and 0s) to keep track of which components an entity has,
/// and alse helps keep track of which entities a system is interested in.
///</summary>
typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent
{
protected:
	inline static int nextId = 0;
};

// Used to assign a unique ID to a component type
template <typename T>
class Component: public IComponent
{
	friend class System; // System can access GetId()

private:
	// Returns the unique ID for this component type
	static int GetId()
	{
		static auto id = nextId++;
		return id;
	}
};

class Entity
{
public:
	Entity(int id) : id(id) {}
	Entity(const Entity& other) : id(other.id) {}
	int GetId() const { return id; }
	//...

	bool operator == (const Entity& other) const { return id == other.id; }
	bool operator != (const Entity& other) const { return id != other.id; }
	Entity& operator= (const Entity& other) const = default;

private:
	int id;
};


///<summary>
/// The System processes the entities that have the components it requires.
/// </summary>
class System
{
private:
	Signature componentSignature;
	std::vector<Entity> entities;

public:
	System() = default;
	~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityToSystem(Entity entity);
	std::vector<Entity> GetSystemEntities() const
	{
		return entities;
	}
		
	const Signature& GetComponentSignature() const
	{
		return componentSignature;
	}

	// Define the components that the system requires
	template <typename TComponent>
	inline void RequireComponent()
	{
		const auto componentId = Component<TComponent>::GetId();
		componentSignature.set(componentId);
	}
};

class Registry
{

};

#endif