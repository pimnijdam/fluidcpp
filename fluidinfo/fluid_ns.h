#ifndef _FLUID_NS_
#define _FLUID_NS_

#include "generic_stuff.h"
#include "fluid_security.h"
#include "fluid_session_details.h"


/*@@ TODO: operator[int], operator[string] to get namespace by index or string, put some methods in the base impl */
namespace fluidinfo{
	class Namespace: public SessionDetails
	{
		public:
			Namespace():_name(""), _description(""), _id(""), _uri(""), fresh(true), _nonexistent(true) 
			{  
			  _nameChain = "";
			}
			
			Namespace(std::string name,std::string description=""):_name(name),_description(description),_id(""), _uri(""), fresh(true), _nonexistent(true) 
			{ 
			 // _nameChain = parentSession->AuthObj.username + "/" + _name;
			}
			
			void setParentSession(Session *p) {
			  SessionDetails::setParentSession(p);
			    _nameChain = parentSession->AuthObj.username + "/" + _name;
			}
			
			virtual ~Namespace();
			

			void set(std::string name, std::string description="") { 
			  static bool isset = false;
			  if ( !isset ) {
			    _name = name; 
			    _description=description; fresh=true; 
			    _nameChain = parentSession->AuthObj.username + "/" + _name;
			    isset = true;
			  }
			}
	
			std::string getName() const { return _name; }
			std::string getDescription() const { return _description; }
			std::string getId() const { return _id; }
			std::string getUri() const { return _uri; }
			
			void getSubNamespaceInfo(const std::string& subns, Namespace& ret, bool returnDescription=true, bool returnTags=true, bool returnNamespaces=true);
			
			void getSecurity(security&, categories categ);
			void setSecurity(security&, categories categ);
			void Delete();
			void updateDescription(const std::string &description);
			void create(const std::string& parentNs="");
			void setError(std::string err) {
			   // ((SessionDetails*)this)->setError(err); SEGFAULT here
			   
			    std::cout << "setError called " << std::endl;
			    _err = err;
			   if ( _err == "NamespaceAlreadyExists" ) {
			     fresh = false;
			     _nonexistent = false;
			   }
			   
			   if ( _err == "JsonParseError" ) {
			      
			   }
			   
			   if ( _err == "NonexistentNamespace" ) {
			      _nonexistent = true;
			   }
			   
			}
			bool isFresh() {
			    return fresh;
			}
			
			bool exists() {
			    return !_nonexistent;
			}

		protected:
			std::string _name;
			std::string _nameChain;
			std::string _description;
		
			std::string _id;
			std::string _uri;
			
			std::vector<std::string> _tagNames;
			std::vector<std::string> _namespaceNames;
		
			security _securityObj;
			
			bool fresh;
			bool _nonexistent;
			
			
			//callbacks
			static size_t FWcreate(void *ptr, size_t size, size_t nmemb, void* p);
			static size_t FWgetSubNamespaceInfo(void *ptr, size_t size, size_t nmemb, void* p);
			static size_t FWupdateDescription(void *ptr, size_t size, size_t nmemb, void* p);

		
	}; 
	
	
}
#endif
