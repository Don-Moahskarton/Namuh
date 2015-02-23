/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS_SANDBOX_CONSTRUCTION // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS_SANDBOX_CONSTRUCTION

struct NxOgre_NamedJoint
{
	string name;
	NxOgre::Joint* joint;
};

class Construction
{
public:

	Construction(void); 
	~Construction(void); 
	Critter::Body* root; // TODO : Gestion de ce node dans la sandbox
	std::vector<ConstructionElement*> element;
	std::vector<NxOgre_NamedJoint> joint;
	std::map<NxOgre_NamedJoint*, std::pair<std::vector<Ogre::SceneNode*>, std::vector<Ogre::SceneNode*>>> ropeList;	// Each namedjoint have two list of ropes : one per linked body
	string name;

	void CreateFixedLink(ConstructionRigidElement* FirstBody, ConstructionRigidElement* SecondBody, string name = "", bool buildRopes = false, Ogre::Vector3 ropePosA = Ogre::Vector3::ZERO, Ogre::Vector3 ropePosB = Ogre::Vector3::ZERO, float ropeRadius = 0.0f); // build rope is only made for log
	//void CreateRevoluteLink(NxOgre::RigidBody* FirstBody, NxOgre::RigidBody* SecondBody, string name = ""); // DO NOT IMPLEMENT (solved by a triangle of logs where a fourth log can goes in)

	void DestroyJoint(NxOgre::Joint* Joint);

	// Mechanic functions
	void Rotate(Ogre::Quaternion NewRelativeOrentation);
	void setOrientation(Ogre::Quaternion NewAbsoluteOrentation);

	void Move(Ogre::Vector3 NewRelativePosition);
	void setPosition(Ogre::Vector3 NewAbsolutePosition);

	ConstructionElement* getElementFromBody(NxOgre::RigidBody*);

	NamedDataPool* Serialize();

	static Construction* MakeFromDataPool(NamedDataPool&);
};

#endif