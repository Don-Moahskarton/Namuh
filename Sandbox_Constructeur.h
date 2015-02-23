/**=====================================================================================
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
---------------------------- Namuh Test Child Header File ------- v alpha1 ------------
*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*
=====================================================================================**/
#ifndef CLASS__SANDBOX_CONSTRUCTEUR // Pour éviter la redondance des déclarations de cette classe (on l'inclus plusieurs fois)
#define CLASS__SANDBOX_CONSTRUCTEUR

enum Construction_mode
{
	NONE	= 0,
	DELOBJ,
	RONDIN,
	ROCHER,
	DISK,
	TRIANGLE,
	ANIMAL_HIDE,
	CREATE_LINK_PICK_FIRST,
	CREATE_LINK_PICK_SECOND,
};

enum Cursor3DType
{
	INVALID			= 0,
	CAST_TERRAIN	= 1,
	CAST_OBJECT		= 2
};

class Sandbox_Constructeur
{
	private:

		Construction_mode CurrentBuildTool;
		int		Construction_num,
				hideCornerNum;		// Redondancy with std::vector<std::pair<Ogre::Vector3, NxOgre::RigidBody*>> hideCornersAttachment::size(), but the code is clearer if separated

		TrunkDescription mTrunkDesc;
		MenhirDescription mMenhirDesc;
		DiskDescription mDiskDesc;

	public:

		bool	initialised,
				isDrawing; // true is user is holding mouse button to draw 

		Cursor3DType	Cursor3D_Type;

		Entity		*RondinCursor,
					*RocherCursor,
					*DiskCursor,
					*SphereCursor;
		SceneNode	*RondinCursor_n,
					*RocherCursor_n,
					*DiskCursor_n,
					*TriangleCursor_n,
					*SphereCursor_n;

		Vector3		pos,
					clikedPos;
		
		std::vector<std::pair<Ogre::Vector3, NxOgre::RigidBody*>> hideCornersAttachment; // store position and body to attach fur

		float	orientation_rondin,
				diametre_rondin,
				longeur_rondin,
				diskOrientation,
				diskLength,
				diskRadius,
				triangleOrientation,
				triangleDirection,
				triangleLength,
				triangleRadius;

		NxOgre::RigidBody	*ClickedBody,
							*drawBodyToLinkA,
							*drawBodyToLinkB;

		std::vector<Construction*> BuiltCstr;



		Sandbox_Constructeur(void); //Constructeur
	   ~Sandbox_Constructeur(void); //Destructeur

		void intitalise(void);
		void update(void);
		void UpdateConstructeur(void);
		void UpdateBuildUI(void);
		void UpdateDeleteUI(void);
		void UpdateCreateLinkPickFirstUI(void);
		void UpdateCreateLinkPickSecondUI(void);

		void setConstructionMode(Construction_mode NewMode);
		Construction_mode getConstructionMode(void);

		void setConstruction_num(int Construction_num);
		int getConstruction_num(void);

		void sethideCornerNum(int hideCornerNum);
		int gethideCornerNum(void);
		
		void refreshLinkLst(void);
		
		Ogre::Vector3 ComputeCursor3DPos(float offset = 0.25f);
		void NewConstruction(void);
		void destroy(void);

	   //   void create_new_contruction(void);
	   //	void switch_to_construction(unsigned int);

};
extern Sandbox_Constructeur Constructeur;

#endif
