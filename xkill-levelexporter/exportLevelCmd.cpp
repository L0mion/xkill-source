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
        MStatus doIt( const MArgList& );
        MStatus redoIt();
        MStatus undoIt();
        bool isUndoable() const;
        static void* creator();
};
exportLevel::exportLevel()
{
}
exportLevel::~exportLevel()
{
}

MStatus exportLevel::doIt( const MArgList& args)
{
	bool verbose = false;
	for( unsigned int i=0; i < args.length();++i )
	{
		if( args.asString(i) == "-v" || args.asString(i) == "-verbose" )
		{
			verbose = true;
		}
	}

	MString output;
	MString command;
	MObject obj;

	////////////////////////////////////////////////////////////////////////////////////////////////
	// INDEXING BEGINS HERE ////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////
	// SAVE USER SELECTIONS
	///////////////////////
	MSelectionList userSelection;
	MGlobal::getActiveSelectionList(userSelection); // Save the users selections for later restoring
	MGlobal::clearSelectionList();


	/////////////////////
	// SELECT RIGIDBODIES
	/////////////////////
	MSelectionList rigidbodies;
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
	MGlobal::displayInfo(output);  // Print number of rigidbodies found
	MGlobal::clearSelectionList();
	

	///////////////////////////////
	// SELECT MESHES BY RIGIDBODIES
	///////////////////////////////
	MSelectionList meshes;
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
	MGlobal::displayInfo(output);  // Print number of meshes found
	MGlobal::clearSelectionList();


	//////////////////////////////////
	// SELECT INSTANCES BY RIGIDBODIES
	//////////////////////////////////
	MSelectionList instances;
	for( unsigned int i = 0; i < rigidbodies.length(); i++)
	{
		rigidbodies.getDependNode(i,obj);
		MFnDependencyNode fn(obj);
		output = fn.name().asChar();
		output = output.substring(0,output.length()-10) + "*";
		MGlobal::selectByName( output ); // Select the objects which correspont to a RigidBody
	}
	MGlobal::getActiveSelectionList(instances);
	output = "Number instances found by RigidBodies: ";
	output = output + instances.length();
	if(verbose)
	{
		MGlobal::displayInfo(output);  // Print number of instances found
	}
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
	MGlobal::displayInfo(output);  // Print number of meshes found
	MGlobal::clearSelectionList();


	///////////////////////
	// SELECT GAME ENTITIES
	///////////////////////
	MSelectionList spawns;
	MSelectionList lights;
	MSelectionList ammos;
	MSelectionList hacks;

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
	MGlobal::displayInfo(output);  // Print number of spawns found
	

	////////////////////////////////////////////////////////////////////////////////////////////////
	// EXPORTING BEGINS HERE ///////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////
	// EXPORT RIGIDBODIES TO .bullet
	////////////////////////////////
	MTransformationMatrix::RotationOrder order;
	MVector* rbTranslation = new MVector[rigidbodies.length()];
	double** rbRotation = new double*[rigidbodies.length()];
	for(unsigned int i = 0; i < rigidbodies.length(); i++)
	{
		rigidbodies.getDependNode(i,obj);
		MFnTransform fn(obj);
		MObject obj2;
		MFnTransform fn2(obj2);

		// Get transform for rigidbody
		rbTranslation[i] = fn.getTranslation(MSpace::kTransform);
		rbRotation[i] = new double[3];
		fn.getRotation(rbRotation[i],order);

		// Get transform for mesh
		MVector	mTranslation = fn2.getTranslation(MSpace::kTransform);
		double mRotation[3];
		fn2.getRotation(mRotation,order);

		// Get "origo" tranform
		MVector oTranslation = rbTranslation[i] - mTranslation;
		double* oRotation = new double[3];
		oRotation[0] = rbRotation[i][0] - mRotation[0];
		oRotation[1] = rbRotation[i][1] - mRotation[1];
		oRotation[2] = rbRotation[i][2] - mRotation[2];

		// Set rigidbody to "origo" transform
		fn.setTranslation(oTranslation,MSpace::kTransform);
		fn.setRotation(oRotation,order);

	}
	command = "file -force -options \"v=0;\" -type \"Bullet Physics export\" -pr -ea \"C:/Users/Nils/Desktop/testexport/TestArena.bullet\";";
	MGlobal::executeCommand(command,output,false);
	for(unsigned int i = 0; i < rigidbodies.length(); i++)
	{
		rigidbodies.getDependNode(i,obj);
		MFnTransform fn(obj);
		
		// Set rigidbody to "rigidbody" transform
		fn.setTranslation(rbTranslation[i],MSpace::kTransform);
		fn.setRotation(rbRotation[i],order);
	}
	if(verbose)
	{
		output = "Rigidbodies exported to: " + output;
		MGlobal::displayInfo(output);
	}
	output = rigidbodies.length();
	output = output + " rigidbodies have been exported to .bullet";
	MGlobal::displayInfo(output);


	////////////////////////
	// EXPORT MESHES TO .obj
	////////////////////////
	MGlobal::clearSelectionList();
	for( unsigned int i = 0; i < meshes.length(); i++)
	{
		meshes.getDependNode(i,obj);
		MFnTransform fn(obj);

		// Get transform for mesh
		MVector	mTranslation = fn.getTranslation(MSpace::kTransform);
		double mRotation[3];
		fn.getRotation(mRotation,order);
		MVector noTrans;
		double noRot[3] = {0.0,0.0,0.0};
		fn.setTranslation(noTrans,MSpace::kTransform);
		fn.setRotation(noRot,order);

		//
		MGlobal::clearSelectionList();
		command = "select -r ";
		command = command + fn.name();
		MGlobal::executeCommand(command,false);
		command = "file -force -options \"groups=1;ptgroups=0;materials=1;smoothing=0;normals=1\" -typ \"OBJexport\" -pr -es \"C:/Users/Nils/Desktop/testexport/";
		command = command + fn.name().asChar();
		command = command + ".obj\";";
		MGlobal::executeCommand(command,output,false);
		output = "Mesh exported to: " + output;
		if(verbose)
		{
			MGlobal::displayInfo(output);
		}
		fn.setTranslation(mTranslation,MSpace::kTransform);
		fn.setRotation(mRotation,order);
	}
	output = meshes.length();
	output = output + " meshes have been exported to .obj's";
	MGlobal::displayInfo(output);
	MGlobal::clearSelectionList();

	//////////////////////////////
	// EXPORT ENTITIES TO .mdldesc
	//////////////////////////////
	
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
		row = row + ".obj 0\n";
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
		row = row + fn.name().substring(4,fn.name().length());
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
		row = row + fn.name().substring(4,fn.name().length());
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
		infile.open(path.asChar());
		while(!infile.eof())
		{
			std::string row_;
			MString row;
			std::getline(infile,row_);
			row = row_.c_str();
			if(row.substring(0,1) == "g ")
			{
				unsigned int limit = row_.find(" ",2);
				row = row.substring(0,limit);
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

	//////////////////////////
	// RESTORE USER SELECTIONS
	//////////////////////////
	MGlobal::clearSelectionList();
	MGlobal::setActiveSelectionList(userSelection); // Restore the users selections
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
MStatus initializePlugin( MObject obj )
{
    MFnPlugin plugin( obj, "XKill-team", "0.1", "Any" );
    plugin.registerCommand( "exportLevel", exportLevel::creator );
    return MS::kSuccess;
}
MStatus uninitializePlugin( MObject obj )
{
    MFnPlugin plugin( obj );
    plugin.deregisterCommand( "exportLevel" );
    return MS::kSuccess;
}