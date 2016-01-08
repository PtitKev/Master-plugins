/*
 * WebBrowser.h
 *
 *  Created on: Jan 6, 2014
 *      Author: denia
 */

#ifndef WEBBROWSER_H
#define WEBBROWSER_H

#include "jsoncpp/json/json.h"

namespace ydleMaster {

class WebBrowser {
public:
	WebBrowser(std::string url);
	virtual ~WebBrowser();
	Json::Value doGet(std::string url, std::string parameters);
	Json::Value doPost(std::string url, Json::Value  *post_data);
	Json::Value doPost(std::string url, std::string  post_data);
	// Callback function for handle the data received from a request
	static size_t responseToJsonObjectCallback( char *ptr, size_t size, size_t nmemb, void *userdata);
private:

	std::string url;
};

} ; // namespace ydleMaster

#endif // WEBBROWSER_H