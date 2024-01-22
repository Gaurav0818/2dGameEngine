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

class Component
{

};

class Entity
{
private:
	int id;

public:
	Entity(int id) : id(id) {}
	int GetId() const { return id; }
	//...
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
	std::vector<Entity> GetSystemEntities() const;
	Signature& GetComponentSignature() const;
};

class Registry
{

};

#endif