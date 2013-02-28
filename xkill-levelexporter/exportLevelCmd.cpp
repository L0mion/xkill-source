#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxCommand.h>
#include <maya/MIOStream.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnMesh.h>
#include <maya/MFnTransform.h>
#include <maya/MQuaternion.h>

class exportLevel : public MPxCommand
{
    public:
        exportLevel(); 
        virtual ~exportLevel(); 
        MStatus doIt( const MArgList& ); //!< Handle the call to the command and execute
        MStatus redoIt(); //!< Redoes the last doIT
        MStatus undoIt(); //!< Undoes the last doIt
        bool isUndoable() const; //!< Tells maya wheter or not the command is undoable
		
        static void* creator();  //!< Allocates an instance of the command

		bool verbose;  //!< tells the command to print full information
		bool clearhist;  //!< tells the command to clear the history in the script editor and only show this commands output

		void selectStuff(bool verboseSelect);  //!< Select the objets from the scene
		void commandpart1();  //!< Displace rigidbodies before export
		void commandpart2();  //!< export rigidbodies
		void commandpart3();  //!< restore displaced rigidbodies
		void commandpart4(); //!< export objs
		void commandpart5(); //!< export .mdldesc

		bool quad;

		MSelectionList rigidbodies;  //!< holds selected rigidbodies in scene
		MSelectionList meshes;  //!< holds selected meshes in scene
		MSelectionList instances;  //!< holds selected instances in scene
		MSelectionList spawns;  //!< holds selected spawns in scene
		MSelectionList lights;  //!< holds selected lights in scene
		MSelectionList ammos;  //!< holds selected ammos in scene
		MSelectionList hacks;  //!< holds selected hacks in scene
};
exportLevel::exportLevel()
{
}
exportLevel::~exportLevel()
{
}

void exportLevel::selectStuff(bool verboseSelect)
{
	MString output;
	MString command;
	MObject obj;

	/////////////////////
	// SELECT RIGIDBODIES
	/////////////////////
	
	MGlobal::selectByName( "*RigidBody*"); // Select all nodes with RigidBody in their name
	MGlobal::getActiveSelectionList(rigidbodies);
	for( unsigned int i = 0; i < rigidbodies.length(); i++)
	{
		rigidbodies.getDependNode(i,obj);
		MFnDependencyNode fn(obj);
		
		output = fn.name().asChar();
		std::string t = output.asChar();
		if(t.find("Shape") != std::string::npos)
		{
			rigidbodies.remove(i--); // Remove all nodes that also contain Shape
		}
		else if(verbose)
		{
			output = "\t" + output;
			MGlobal::displayInfo(output);  // Print name of found RigidBody
		}
	}
	output = "Number of rigidbodies in scene: ";
	output = output + rigidbodies.length();
	if(verboseSelect)
		MGlobal::displayInfo(output);  // Print number of rigidbodies found
	MGlobal::clearSelectionList();
	

	///////////////////////////////
	// SELECT MESHES BY RIGIDBODIES
	///////////////////////////////
	
	for( unsigned int i = 0; i < rigidbodies.length(); i++)
	{
		rigidbodies.getDependNode(i,obj);
		MFnDependencyNode fn(obj);
		output = fn.name().asChar();
		output = output.substring(0,output.length()-10);
		MGlobal::selectByName( output ); // Select the objects which correspont to a RigidBody
	}
	MGlobal::getActiveSelectionList(meshes);
	output = "Number of meshes in scene: ";
	output = output + meshes.length();
	if(verboseSelect)
		MGlobal::displayInfo(output);  // Print number of meshes found
	MGlobal::clearSelectionList();


	//////////////////////////////////
	// SELECT INSTANCES BY RIGIDBODIES
	//////////////////////////////////
	for( unsigned int i = 0; i < rigidbodies.length(); i++)
	{
		rigidbodies.getDependNode(i,obj);
		MFnDependencyNode fn(obj);
		output = fn.name().asChar();
		output = output.substring(0,output.length()-10) + "*";
		MGlobal::selectByName( output ); // Select the objects which correspont to a RigidBody
	}
	MGlobal::getActiveSelectionList(instances);
	for( unsigned int i = 0; i < instances.length(); i++)
	{
		instances.getDependNode(i,obj);
		MFnDependencyNode fn(obj);
		
		output = fn.name().asChar();
		std::string t = output.asChar();
		if(t.find("Shape") != std::string::npos)
		{
			instances.remove(i--); // Remove all nodes that also contain Shape
		}
		else if(t.find("RigidBody") != std::string::npos)
		{
			instances.remove(i--); // Remove all nodes that also contain RigidBody
		}
		else if(verbose)
		{
			output = "\t" + output;
			MGlobal::displayInfo(output);  // Print name of found mesh
		}
	}
	output = "Number of instances in scene: ";
	output = output + instances.length();
	if(verboseSelect)
		MGlobal::displayInfo(output);  // Print number of meshes found
	MGlobal::clearSelectionList();


	///////////////////////
	// SELECT GAME ENTITIES
	///////////////////////
	

	MGlobal::selectByName( "ammo*"); // Select all nodes with ammo in their name
	MGlobal::getActiveSelectionList(ammos);
	MGlobal::clearSelectionList();
	for( unsigned int i = 0; i < ammos.length(); i++)
	{
		ammos.getDependNode(i,obj);
		MFnDependencyNode fn(obj);
		std::string t = fn.name().asChar();
		if(t.find("Shape") != std::string::npos)
		{
			ammos.remove(i--); // Remove all nodes that also contain Shape
		}
		else if(fn.name().substring(0,3) != "ammo")
		{
			ammos.remove(i--);
		}
		else if( fn.name().substring(4,fn.name().length()).length() > 0)
		{
			if(!(fn.name().substring(4,6) == "EXP" ||
				  fn.name().substring(4,6) == "SHA" ||
				  fn.name().substring(4,6) == "SIN"))
			{
				ammos.remove(i--);
			}
		}
		
	}
	output = "Number of ammo in scene: ";
	output = output + ammos.length();
	if(verboseSelect)
		MGlobal::displayInfo(output);  // Print number of ammo found

	MGlobal::selectByName( "hack*"); // Select all nodes with hack in their name
	MGlobal::getActiveSelectionList(hacks);
	MGlobal::clearSelectionList();
	for( unsigned int i = 0; i < hacks.length(); i++)
	{
		hacks.getDependNode(i,obj);
		MFnDependencyNode fn(obj);
		std::string t = fn.name().asChar();
		if(t.find("Shape") != std::string::npos)
		{
			hacks.remove(i--); // Remove all nodes that also contain Shape
		}
		else if(fn.name().substring(0,3) != "hack")
		{
			hacks.remove(i--);
		}
		else if( fn.name().substring(4,fn.name().length()).length() > 0)
		{
			if(!(fn.name().substring(4,6) == "HEL" ||
				  fn.name().substring(4,6) == "SPE"))
			{
				hacks.remove(i--);
			}
		}
	}
	output = "Number of hacks in scene: ";
	output = output + hacks.length();
	if(verboseSelect)
		MGlobal::displayInfo(output);  // Print number of hacks found

	MGlobal::selectByName( "light*"); // Select all nodes with light in their name
	MGlobal::getActiveSelectionList(lights);
	MGlobal::clearSelectionList();
	for( unsigned int i = 0; i < lights.length(); i++)
	{
		lights.getDependNode(i,obj);
		MFnDependencyNode fn(obj);
		std::string t = fn.name().asChar();
		if(t.find("Shape") != std::string::npos)
		{
			lights.remove(i--); // Remove all nodes that also contain Shape
		}
		else if(fn.name().substring(0,4) != "light")
		{
			lights.remove(i--);
		}
		else if(fn.name().substring(5,fn.name().length()).length() > 0)
		{
			if(!(fn.name().substring(5,7) == "AMB" ||
					  fn.name().substring(5,7) == "DIR" ||
					  fn.name().substring(5,7) == "POI" ||
					  fn.name().substring(5,7) == "SPO"))
			{
				lights.remove(i--);
			}
		}
		
	}
	output = "Number of lights in scene: ";
	output = output + lights.length();
	if(verboseSelect)
		MGlobal::displayInfo(output);  // Print number of lights found

	MGlobal::selectByName( "spawn*"); // Select all nodes with spawn in their name
	MGlobal::getActiveSelectionList(spawns);
	MGlobal::clearSelectionList();
	for( unsigned int i = 0; i < spawns.length(); i++)
	{
		spawns.getDependNode(i,obj);
		MFnDependencyNode fn(obj);
		std::string t = fn.name().asChar();
		if(t.find("Shape") != std::string::npos)
		{
			spawns.remove(i--); // Remove all nodes that also contain Shape
		}
		else if(fn.name().substring(0,4) != "spawn")
		{
			spawns.remove(i--);
		}
		else if(fn.name().substring(5,fn.name().length()).length() > 0)
		{
			if(!fn.name().substring(5,fn.name().length()).isInt())
			{
				spawns.remove(i--);
			}
		}
		
	}
	output = "Number of spawns in scene: ";
	output = output + spawns.length();
	if(verboseSelect)
		MGlobal::displayInfo(output);  // Print number of spawns found
}

void exportLevel::commandpart1()
{
	MString output;
	if(verbose)
	{
		MGlobal::displayInfo("Command part 1 - Displace rigidbodies for export ");
	}
	MTransformationMatrix::RotationOrder order;
	MObject rigidbody;
	MObject mesh;
	
	MVector rigidbodyTranslation, meshTranslation, resultTranslation;
	double rigidbodyRotation[3], meshRotation[3], resultRotation[3];
	for(unsigned int i = 0; i < rigidbodies.length(); i++)
	{
		// Get rigidbody and mesh
		rigidbodies.getDependNode(i,rigidbody);
		meshes.getDependNode(i,mesh);
		MFnTransform fnRigidBody(rigidbody);
		MFnTransform fnMesh(mesh);
		// Print their name
		if(verbose)
		{
			MGlobal::displayInfo(fnRigidBody.name());
			MGlobal::displayInfo(fnMesh.name());
		}

		// Get transform for rigidbody
		rigidbodyTranslation = fnRigidBody.getTranslation(MSpace::kTransform);
		fnRigidBody.getRotation(rigidbodyRotation,order);
		if(verbose)
		{
			output = "RigidBody transformation ";
			output = output + rigidbodyTranslation.x;
			output = output + " ";
			output = output + rigidbodyTranslation.y;
			output = output + " ";
			output = output + rigidbodyTranslation.z;
			output = output + " ";
			output = output + rigidbodyRotation[0];
			output = output + " ";
			output = output + rigidbodyRotation[1];
			output = output + " ";
			output = output + rigidbodyRotation[2];
			MGlobal::displayInfo(output);
		}
		// Get transform for mesh
		meshTranslation = fnMesh.getTranslation(MSpace::kTransform);
		fnMesh.getRotation(meshRotation,order);
		if(verbose)
		{
			output = "Mesh transformation ";
			output = output + meshTranslation.x;
			output = output + " ";
			output = output + meshTranslation.y;
			output = output + " ";
			output = output + meshTranslation.z;
			output = output + " ";
			output = output + meshRotation[0];
			output = output + " ";
			output = output + meshRotation[1];
			output = output + " ";
			output = output + meshRotation[2];
			MGlobal::displayInfo(output);
		}
		// Get resulting transform and set to rigidbody
		resultTranslation = rigidbodyTranslation - meshTranslation;
		resultRotation[0] = rigidbodyRotation[0] - meshRotation[0];
		resultRotation[1] = rigidbodyRotation[1] - meshRotation[1];
		resultRotation[2] = rigidbodyRotation[2] - meshRotation[2];
		if(verbose)
		{
			output = "Result transformation ";
			output = output + meshTranslation.x;
			output = output + " ";
			output = output + meshTranslation.y;
			output = output + " ";
			output = output + meshTranslation.z;
			output = output + " ";
			output = output + meshRotation[0];
			output = output + " ";
			output = output + meshRotation[1];
			output = output + " ";
			output = output + meshRotation[2];
			MGlobal::displayInfo(output);
		}
		
		fnRigidBody.setTranslation(resultTranslation,MSpace::kTransform);
		fnRigidBody.setRotation(resultRotation,order);
	}
}
void exportLevel::commandpart2()
{
	if(verbose)
	{
		MGlobal::displayInfo("Command part 2 - Export .bullet");
	}
	MString output;
	MString command;
	command = "file -force -options \"v=0;\" -type \"Bullet Physics export\" -pr -ea \"C:/Users/Nils/Desktop/testexport/TestArena.bullet\";";
	MGlobal::executeCommand(command,output,false);
	if(verbose)
	{
		output = "Rigidbodies exported to " + output;
		MGlobal::displayInfo(output);
	}
}
void exportLevel::commandpart3()
{
	MString output;
	if(verbose)
	{
		MGlobal::displayInfo("Command part 3 - Restore displacements");
	}
	MTransformationMatrix::RotationOrder order;
	MObject rigidbody;
	MObject mesh;
	
	MVector rigidbodyTranslation, meshTranslation, resultTranslation;
	double rigidbodyRotation[3], meshRotation[3], resultRotation[3];
	for(unsigned int i = 0; i < rigidbodies.length(); i++)
	{
		// Get rigidbody and mesh
		rigidbodies.getDependNode(i,rigidbody);
		meshes.getDependNode(i,mesh);
		MFnTransform fnRigidBody(rigidbody);
		MFnTransform fnMesh(mesh);
		// Print their name
		if(verbose)
		{
			MGlobal::displayInfo(fnRigidBody.name());
			MGlobal::displayInfo(fnMesh.name());
		}

		// Get transform for rigidbody
		rigidbodyTranslation = fnRigidBody.getTranslation(MSpace::kTransform);
		fnRigidBody.getRotation(rigidbodyRotation,order);
		if(verbose)
		{
			output = "RigidBody transformation ";
			output = output + rigidbodyTranslation.x;
			output = output + " ";
			output = output + rigidbodyTranslation.y;
			output = output + " ";
			output = output + rigidbodyTranslation.z;
			output = output + " ";
			output = output + rigidbodyRotation[0];
			output = output + " ";
			output = output + rigidbodyRotation[1];
			output = output + " ";
			output = output + rigidbodyRotation[2];
			MGlobal::displayInfo(output);
		}
		// Get transform for mesh
		meshTranslation = fnMesh.getTranslation(MSpace::kTransform);
		fnMesh.getRotation(meshRotation,order);
		if(verbose)
		{
			output = "Mesh transformation ";
			output = output + meshTranslation.x;
			output = output + " ";
			output = output + meshTranslation.y;
			output = output + " ";
			output = output + meshTranslation.z;
			output = output + " ";
			output = output + meshRotation[0];
			output = output + " ";
			output = output + meshRotation[1];
			output = output + " ";
			output = output + meshRotation[2];
			MGlobal::displayInfo(output);
		}
		// Get resulting transform and set to rigidbody
		resultTranslation = rigidbodyTranslation + meshTranslation;
		resultRotation[0] = rigidbodyRotation[0] + meshRotation[0];
		resultRotation[1] = rigidbodyRotation[1] + meshRotation[1];
		resultRotation[2] = rigidbodyRotation[2] + meshRotation[2];
		if(verbose)
		{
			output = "Result transformation ";
			output = output + meshTranslation.x;
			output = output + " ";
			output = output + meshTranslation.y;
			output = output + " ";
			output = output + meshTranslation.z;
			output = output + " ";
			output = output + meshRotation[0];
			output = output + " ";
			output = output + meshRotation[1];
			output = output + " ";
			output = output + meshRotation[2];
			MGlobal::displayInfo(output);
		}
		
		fnRigidBody.setTranslation(resultTranslation,MSpace::kTransform);
		fnRigidBody.setRotation(resultRotation,order);
	}
}
void exportLevel::commandpart4()
{
	MString output;
	MString command;
	if(verbose)
	{
		MGlobal::displayInfo("Command part 4 - Export .obj's");
	}
	MGlobal::clearSelectionList();
	MObject mesh;

	for( unsigned int i = 0; i < meshes.length(); i++)
	{
		MTransformationMatrix::RotationOrder order;
		meshes.getDependNode(i,mesh);
		MFnTransform fnMesh(mesh);

		// Get transform for mesh
		MVector	mTranslation = fnMesh.getTranslation(MSpace::kTransform);
		double mRotation[3];
		fnMesh.getRotation(mRotation,order);
		MVector noTrans;
		double noRot[3] = {0.0,0.0,0.0};
		fnMesh.setTranslation(noTrans,MSpace::kTransform);
		fnMesh.setRotation(noRot,order);

		//
		MGlobal::clearSelectionList();
		command = "select -r ";
		command = command + fnMesh.name();
		MGlobal::executeCommand(command,false);
		command = "file -force -options \"groups=1;ptgroups=0;materials=1;smoothing=0;normals=1\" -typ \"OBJexport\" -pr -es \"C:/Users/Nils/Desktop/testexport/";
		command = command + fnMesh.name().asChar();
		command = command + ".obj\";";
		MGlobal::executeCommand(command,output,false);
		output = "Mesh exported to: " + output;
		if(verbose)
		{
			MGlobal::displayInfo(output);
		}
		fnMesh.setTranslation(mTranslation,MSpace::kTransform);
		fnMesh.setRotation(mRotation,order);
	}
	output = meshes.length();
	output = output + " meshes have been exported to .obj's";
	MGlobal::displayInfo(output);
	MGlobal::clearSelectionList();
}
void exportLevel::commandpart5()
{
	MObject obj;
	MString output;
	MString rows;
	rows = "";
	rows = rows +  "# Use .mdldesc to specify what files are to be loaded and interpreted as level world parts\n";
	rows = rows +  "# LVLDESC-header is marked with prefix 'h'. Header info makes out name of MDLDESC and path to models named in .mdldesc.\n";
	rows = rows +  "# LVLDESC-model is marked with prefix 'm'. Second parameter specifies model ID, and the third the name of said model. Fourth parameter specifies whether of not model is dynamic.\n";
	rows = rows +  "\n";
	rows = rows +  "######################################\n";
	rows = rows +  "# USE _SPACES_ TO SEPERATE ARGUMENTS.#\n";
	rows = rows +  "######################################\n";
	rows = rows +  "\n";
	rows = rows +  "########\n";
	rows = rows +  "#HEADER#\n";
	rows = rows +  "########\n";
	rows = rows +  "h default /\n";
	rows = rows +  "\n";
	rows = rows +  "##############\n";
	rows = rows +  "#LEVEL MODELS#\n";
	rows = rows +  "##############\n";
	rows = rows +  "#m <100 FILENAME 0/1(static/dynamic)\n";
	
	for( unsigned int i = 0; i < meshes.length(); i++)
	{
		meshes.getDependNode(i,obj);
		MFnTransform fn(obj);
		MString row = "m ";
		row = row + (i+100);
		row = row + " ";
		row = row + fn.name();
		row = row + ".obj 0 1\n";
		rows = rows +  row;
	}
	
	rows = rows +  "\n";
	rows = rows +  "################\n";
	rows = rows +  "#WORLD ENTITIES#\n";
	rows = rows +  "################\n";
	rows = rows +  "#w meshID X Y Z QX QY QZ QW\n";
	
	for( unsigned int i = 0; i < meshes.length(); i++)
	{
		MObject obj2;
		meshes.getDependNode(i,obj);
		MFnTransform fn(obj);
		for( unsigned int j = 0; j < instances.length(); j++)
		{
			instances.getDependNode(j,obj2);
			MFnTransform fn2(obj2);

			unsigned int fnlength = fn.name().length();
			if(fn2.name().length() >= fnlength)
			{
				if(fn.name() == fn2.name().substring(0,fnlength-1))
				{
					MVector	mTranslation = fn2.getTranslation(MSpace::kTransform);
					MQuaternion mRotation;
					fn2.getRotationQuaternion(mRotation.x,mRotation.y,mRotation.z,mRotation.w,MSpace::kTransform);
					MString row = "w ";
					row = row + (i+100);
					row = row + " ";
					row = row + mTranslation.x;
					row = row + " ";
					row = row + mTranslation.y;
					row = row + " ";
					row = row + mTranslation.z;
					row = row + " ";
					row = row + mRotation.x;
					row = row + " ";
					row = row + mRotation.y;
					row = row + " ";
					row = row + mRotation.z;
					row = row + " ";
					row = row + mRotation.w;
					row = row + "\n";
					rows = rows +  row;

					if(quad)
					{
						MGlobal::displayInfo(output);
						row = "w ";
						row = row + (i+100);
						row = row + " ";
						row = row + -mTranslation.x;
						row = row + " ";
						row = row + mTranslation.y;
						row = row + " ";
						row = row + mTranslation.z;
						row = row + " ";
						row = row + mRotation.x;
						row = row + " ";
						row = row + mRotation.y;
						row = row + " ";
						row = row + mRotation.z;
						row = row + " ";
						row = row + mRotation.w;
						row = row + "\n";
						rows = rows +  row;

						row = "w ";
						row = row + (i+100);
						row = row + " ";
						row = row + mTranslation.x;
						row = row + " ";
						row = row + mTranslation.y;
						row = row + " ";
						row = row + -mTranslation.z;
						row = row + " ";
						row = row + mRotation.x;
						row = row + " ";
						row = row + mRotation.y;
						row = row + " ";
						row = row + mRotation.z;
						row = row + " ";
						row = row + mRotation.w;
						row = row + "\n";
						rows = rows +  row;

						row = "w ";
						row = row + (i+100);
						row = row + " ";
						row = row + -mTranslation.x;
						row = row + " ";
						row = row + mTranslation.y;
						row = row + " ";
						row = row + -mTranslation.z;
						row = row + " ";
						row = row + mRotation.x;
						row = row + " ";
						row = row + mRotation.y;
						row = row + " ";
						row = row + mRotation.z;
						row = row + " ";
						row = row + mRotation.w;
						row = row + "\n";
						rows = rows +  row;
					}
				}
			}
		}
	}

	rows = rows +  "\n";
	rows = rows +  "##############\n";
	rows = rows +  "#SPAWN POINTS#\n";
	rows = rows +  "##############\n";
	rows = rows +  "#s X Y Z Radius\n";

	for( unsigned int i = 0; i < spawns.length(); i++)
	{
		spawns.getDependNode(i,obj);
		MFnTransform fn(obj);
		MVector	mTranslation = fn.getTranslation(MSpace::kTransform);
		MString row = "s ";
		row = row + mTranslation.x;
		row = row + " ";
		row = row + mTranslation.y;
		row = row + " ";
		row = row + mTranslation.z;
		row = row + " 1\n";
		rows = rows +  row;
	}

	rows = rows +  "\n";
	rows = rows + "#######\n";
	rows = rows + "#HACKS#\n";
	rows = rows + "#######\n";
	rows = rows + "#c TYPE X Y Z\n";

	for( unsigned int i = 0; i < hacks.length(); i++)
	{
		hacks.getDependNode(i,obj);
		MFnTransform fn(obj);
		MVector	mTranslation = fn.getTranslation(MSpace::kTransform);
		MString row = "c ";
		row = row + fn.name().substring(4,6);
		row = row + " ";
		row = row + mTranslation.x;
		row = row + " ";
		row = row + mTranslation.y;
		row = row + " ";
		row = row + mTranslation.z;
		row = row + "\n";
		rows = rows +  row;
	}

	rows = rows +  "\n";
	rows = rows + "######\n";
	rows = rows + "#AMMO#\n";
	rows = rows + "######\n";
	rows = rows + "#a TYPE X Y Z\n";

	for( unsigned int i = 0; i < ammos.length(); i++)
	{
		ammos.getDependNode(i,obj);
		MFnTransform fn(obj);
		MVector	mTranslation = fn.getTranslation(MSpace::kTransform);
		MString row = "c ";
		row = row + fn.name().substring(4,6);
		row = row + " ";
		row = row + mTranslation.x;
		row = row + " ";
		row = row + mTranslation.y;
		row = row + " ";
		row = row + mTranslation.z;
		row = row + "\n";
		rows = rows +  row;
	}

	rows = rows +  "\n";
	rows = rows + "########\n";
	rows = rows + "#LIGHTS#\n";
	rows = rows + "########\n";
	rows = rows + "#AR/G/B Ambient rgb\n";
	rows = rows + "#DR/G/B Diffuse rgb\n";
	rows = rows + "#SR/G/B Specular rgb\n";
	rows = rows + "#AX/Y/Z Attenuation xyz\n";
	rows = rows + "#PX/Y/Z Position xyz (not used by dir)\n";
	rows = rows + "#DX/Y/Z Direction xyz (not used by point)\n";
	rows = rows + "#R		Range (not used by dir)\n";
	rows = rows + "#P		SpotPow (Only used by spot)\n";
	rows = rows + "#l TYPE AR AG AB DR  DG  DB  SR  SG  SB AX AY AZ PX PY PZ DX DY DZ R P\n";
	rows = rows + "# 1 = POINT\n";
	rows = rows + "# 2 = DIR\n";
	rows = rows + "# 3 = SPOT\n";
	rows = rows + "l  2   0.8 0.8 0.8 0.2 0.2 0.2 1 1 1  0  0  0  0  0  0  0.57 -0.57 0.57  0 0\n";

	FILE* mdldesc;
	mdldesc = fopen("C:/Users/Nils/Desktop/testexport/TestArena.mdldesc","w");
	if(mdldesc != nullptr)
	{
		fputs( rows.asChar(), mdldesc);
		fclose(mdldesc);
	}



	// Mend maya's outputed obj's
	std::ifstream infile;
	MString path = "C:/Users/Nils/Desktop/testexport/";
	for(unsigned int i = 0; i < meshes.length(); i++)
	{
		meshes.getDependNode(i,obj);
		MFnTransform fn(obj);
		MString filename = path + fn.name();
		MString file = "";
		filename = filename + ".obj";
		infile.open(filename.asChar(),std::ios::in);
		if(infile.is_open())
		{
			while(!infile.eof())
			{
				
				std::string row_;
				MString row;
				std::getline(infile,row_);
				row = row_.c_str();
				if(row.substring(0,1) == "g ")
				{
					unsigned int limit = row_.find(" ",2);
					row = row.substring(0,limit-1);
				}
				file = file + row;
				file = file + "\n";
			}
			infile.close();
			FILE* outfile;
			outfile = fopen(filename.asChar(),"w");
			if(outfile != nullptr)
			{
				fputs( file.asChar(), outfile);
				fclose(outfile);
			}
		}
	}
	

	/*for(unsigned int i = 0; i < lights.length(); i++)
	{
		lights.getDependNode(i,obj);
		MFnTransform fn(obj);
		MVector	mTranslation = fn.getTranslation(MSpace::kTransform);
		MString row = "c ";
		row = row + fn.name().substring(4,fn.name().length());
		row = row + " ";
		row = row + mTranslation.x;
		row = row + " ";
		row = row + mTranslation.y;
		row = row + " ";
		row = row + mTranslation.z;
		row = row + " 1\n";
		rows = rows +  row;
	}*/
}
MStatus exportLevel::doIt( const MArgList& args)
{
	MString command;
	verbose = false;
	clearhist = false;
	int part = -1;
	// Read parameters
	for( unsigned int i=0; i < args.length();++i )
	{
		if( args.asString(i) == "-v" || args.asString(i) == "-verbose" )
		{
			MGlobal::displayInfo("verboseflag\n");
			verbose = true;
		}
		if( args.asString(i) == "-c" || args.asString(i) == "-clear" )
		{
			MGlobal::displayInfo("clearflag\n");
			clearhist = true;
		}
		if( args.asString(i) == "-p" || args.asString(i) == "-part" )
		{
			MGlobal::displayInfo("partflag\n");
			part = args.asInt(++i);
		}
		if( args.asString(i) == "-q" || args.asString(i) == "-quad" )
		{
			MGlobal::displayInfo("quadflag\n");
			quad = true;
		}
	}
	if(part == -1)
	{
		// Save the users selections for later restoring
		MSelectionList userSelection;
		MGlobal::getActiveSelectionList(userSelection); 
		MGlobal::clearSelectionList();
		// Clear history in script editor
		if(clearhist)
		{
			command = "scriptEditorInfo -clearHistory;";
			MGlobal::executeCommand(command);
		}
		// Run subparts of the script
		command = "exportLevel ";
		selectStuff(true);
		command = command + "-v ";
		MGlobal::executeCommand(command + "-p 1",true);
		MGlobal::executeCommand(command + "-p 2",true);
		MGlobal::executeCommand(command + "-p 3",true);
		MGlobal::executeCommand(command + "-p 4",true);
		MGlobal::executeCommand(command + "-p 5",true);
		// RESTORE USER SELECTIONS
		MGlobal::clearSelectionList();
		MGlobal::setActiveSelectionList(userSelection);
	}
	else
	{
		// Run selection of things
		selectStuff(true);
		// Run the correct part
		switch(part)
		{
		case 1:
			commandpart1();
			break;
		case 2:
			commandpart2();
			break;
		case 3:
			commandpart3();
			break;
		case 4:
			commandpart4();
			break;
		case 5:
			commandpart5();
			break;
		}
	}
	return MS::kSuccess;
}
MStatus exportLevel::redoIt()
{
    return MS::kSuccess;
}
MStatus exportLevel::undoIt()
{
    return MS::kSuccess;
}
bool exportLevel::isUndoable() const
{
    return false;
}
void* exportLevel::creator()
{
    return new exportLevel();
}
MStatus initializePlugin( MObject obj ) //!< Registers the plugin command data
{
    MFnPlugin plugin( obj, "XKill-team", "0.2", "Any" );
    plugin.registerCommand( "exportLevel", exportLevel::creator );
    return MS::kSuccess;
} 
MStatus uninitializePlugin( MObject obj ) //!< Unregister the plugin command data
{
    MFnPlugin plugin( obj );
    plugin.deregisterCommand( "exportLevel" );
    return MS::kSuccess;
}