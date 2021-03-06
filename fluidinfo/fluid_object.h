/* 
Copyright 2011 Nicolae Florin Petrovici
 
This file is part of the Fluidinfo C++ library. 
For more information, regarding Fluidinfo, visit http://www.fluidinfo.com

Fluidinfo C++ library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Fluidinfo C++ library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Fluidinfo C++ library.  If not, see <http://www.gnu.org/licenses/>.
*/  

#ifndef _FLUID_OBJECT_
#define _FLUID_OBJECT_

#include "generic_stuff.h"
#include "fluid_security.h"
#include "fluid_session_details.h"

//TODO also implement /about functionality here

namespace fluidinfo{
	
class Object: public SessionDetails
{
public:
	typedef std::auto_ptr<Object> Ptr ;	
	
	explicit Object():_id(""),_uri(""),_about(""),
			bufferGetTagValue_(NULL), idx_bufferGetTagValue_(0),
			bufferGetIdsByQuery_(NULL), idx_bufferGetIdsByQuery_(0), 
			bufferGetTagPaths_(NULL), idx_bufferGetTagPaths_(0) { }
	
	explicit Object(const std::string& id):_id(id), _uri(""), _about(""),
			bufferGetTagValue_(NULL), idx_bufferGetTagValue_(0),
			bufferGetIdsByQuery_(NULL), idx_bufferGetIdsByQuery_(0), 
			bufferGetTagPaths_(NULL),idx_bufferGetTagPaths_(0) { }
	
	virtual ~Object();

	void create();

	void setId(const std::string& id) { _id = id; }
	void isAbout(std::string about="") { _about = about; }
	std::string getAbout() { return _about; }
	void setName(std::string name) { _name = name; }
	void delTag(const std::string& tag, const std::string& tagPath="");
	static bool hasTag(const std::string& id, const std::string& tag, Session& );
	
	void putTag(const std::string& tag, const std::string& tagPath, const std::string& value);
	void put(const std::string& tag, const std::string& tagPath, const std::string& filePath);
	
	void setError(std::string err);	
	
	std::string getTagValue(std::string tag);
	std::vector<std::string> getTagPaths();
	std::vector<std::string> getTagPathsFromCache() { return _tagPaths; }
	std::string getTagValueFromCache(const std::string& tag)
	{
		   if ( _tagMap.find(tag) != _tagMap.end() )
				return _tagMap[tag];
			else
				return "";	
	}
	static Ptr getById(const std::string& , const Session& );
	
	//Looks like fluidinfo does not have a function to get the URI based on the ID
	std::string getURI() { return _uri; }
	std::string getId() { return _id; }
	
		
	//this should be static somehow...
	static std::vector<std::string> getIdsByQuery(const std::string& query, Session& session);
	
	std::vector<std::string> ids;

	//should be protected
	std::vector<std::string> _tagPaths;
	//should be protected
	std::map<std::string,std::string> _tagMap; //tag name, tag value
	
protected:
	std::string _id;
	std::string _name;
	std::string _about;
	std::string _uri;
	

	static size_t FWdelTag(void* ptr, size_t size, size_t nmemb, void* p);
	
	static size_t FWhasTag(void* ptr);
	static size_t FWputTag(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWputBlob(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWcreate(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWgetTagValue(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWgetTagPaths(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWgetIdsByQuery(void *ptr, size_t size, size_t nmemb, void* p);

	char *bufferGetTagValue_;
	uint32_t idx_bufferGetTagValue_;
	
	char* bufferGetIdsByQuery_;
	uint32_t idx_bufferGetIdsByQuery_;
	
	char* bufferGetTagPaths_;
	uint32_t idx_bufferGetTagPaths_;
	
	static std::vector<FILE*> openFiles;
	
	std::string delete_request;
	std::string doc;
	
	bool dirty; //if object is out of sync with Fluidinfo
private:
	bool hasTagResponse_;
};

}
#endif
