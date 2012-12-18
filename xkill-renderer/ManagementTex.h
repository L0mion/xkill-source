#ifndef XKILL_RENDERER_TEXMANAGEMENT_H
#define XKILL_RENDERER_TEXMANAGEMENT_H

typedef long HRESULT;

struct ID3D11Device;
struct ID3D11ShaderResourceView;

class Tex;
class TexDesc;
class TexLoader;

#include <map>
#include <vector>

//! Holds textures which are used in shaders. Recieves TexDesc-objects loaded from file, which specifies what textures to load and what their indices are.
/*!
\ingroup xkill-renderer
*/
class ManagementTex
{
public:
	//! Does nothing.
	ManagementTex();
	//! Releases memory allocated by TexManagement, including vector of Tex-objects.
	~ManagementTex();

	//! Creates TexLoader-object, which will then be used to load textures.
	HRESULT init();

	//! Call this function to load textures specified in TexDesc-object.
	/*!
	\param texDesc	Representation of a .texdesc-file, which specifies what textures are to be loaded.
	\param device	Handle to D3D-device object.
	\return			Any error that might've occured.
	*/
	HRESULT handleTexDesc(
		TexDesc*		texDesc,
		ID3D11Device*	device);

	//! Retrieves an srv (for use in shader) based on an index.
	/*!
	\param texID	ID of texture.
	\return			Returns nullptr if specified texture wasn't found.
	*/
	ID3D11ShaderResourceView* getTexSrv(const unsigned int texID) const;
	//! Gets index of texture in vector holding textures based on texture ID.
	/*!
	\param texID	ID of texture.
	\return			Index of texture in vector holding Tex-objects. Will be -1 if no index found.
	*/
	int getTexsIndex(const unsigned int texID) const;
protected:
private:
	//! Creates a texture-object.
	/*! Calls TexLoader to load srv from file. If method is sucessful, srv along with ID will be stored in TexManagement.
	\param texID		ID of texture.
	\param texPath		Path up to file.
	\param texFileName	Name of file.
	\param device		Handle to D3D-device.
	\return				Whether or not method was sucessful.
	\sa pushTex
	*/
	HRESULT createTex(
		unsigned int	texID, 
		std::string		texPath, 
		std::string		texFileName,
		ID3D11Device*	device);
	//! Based on arguments, this method will create a Tex-type object, and store it in TexManagement for future use.
	void pushTex(
		unsigned int				texID,
		ID3D11ShaderResourceView*	srv);

	/*intermediate*/
	TexLoader* texLoader_;				//!< Loader used to load textures from file.

	/*result*/
	std::vector<Tex*> texs_;			//!< Textures loaded from file.
	std::map<
		unsigned int, 
		unsigned int> texIDtoIndex_;	//!< Map used to map texture IDs to indices in texs_-vector.
};

#endif //XKILL_RENDERER_TEXMANAGEMENT_H