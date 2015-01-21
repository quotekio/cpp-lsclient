#include "lssubscription.hpp"

LSSubscription::LSSubscription(std::string item_t, 
	                           std::vector<std::string> obj_ids, 
	                           std::vector<std::string> flist) {
  item_type = item_t;
  object_ids = obj_ids;
  fields= flist;
}

std::string LSSubscription::getItemType() {
  return item_type;
}

std::vector<std::string> LSSubscription::getObjectIds(){
  return object_ids;
}

std::vector<std::string> LSSubscription::getFields(){
  return fields;
}
