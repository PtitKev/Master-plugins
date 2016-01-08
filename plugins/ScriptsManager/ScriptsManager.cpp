/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "ScriptsManager.h"
#include "LogDestination.h"
#include <iostream>
#include "Dir.h"
#include "SettingsParser.h"
using namespace ydleMaster ;
using namespace std ;

void ScriptsManager::Init (Kernel * k)
{
//	memset (_nodes, 0, sizeof (_nodes)) ;

	// search for node affection in config file
try {

   // memorize list of protocols

        pFeatures = &(k->Features()) ;

}
catch (const runtime_error & e)  {
	YDLE_FATAL << "ScriptsManager::Init FAILED !!! : " << e.what() ;
}
}
int	ScriptsManager::ReloadScript (string script_file)
{
    YDLE_DEBUG <<  script_file  <<std::endl;
    string script = PARAM_STR("lua-scripts.scripts_dir")+ "/"+ script_file + ".lua" ;
    for( Kernel::FeatureList::iterator it = pFeatures->begin(); it != pFeatures->end(); ++it) {
		IFeature * feature = *it ;
        //        IFeature  feature = "lua-script";
                //feature->AddScript (script.c_str()) ;
                feature->ReloadScript(script.c_str());
                printf ("\tFeature : %s:AddScript \n", feature->Name().c_str()) ;
        //intf("\t Test \n");
        }
    
	return 1 ;
}
int	ScriptsManager::LoadScript (string script_file)
{
    YDLE_DEBUG <<  script_file  <<std::endl;
    string script = PARAM_STR("lua-scripts.scripts_dir")+ "/"+ script_file + ".lua" ;
    for( Kernel::FeatureList::iterator it = pFeatures->begin(); it != pFeatures->end(); ++it) {
		IFeature * feature = *it ;
        //        IFeature  feature = "lua-script";
                //feature->AddScript (script.c_str()) ;
                feature->LoadScript(script.c_str());
                printf ("\tFeature : %s:LoadScript \n", feature->Name().c_str()) ;
        //intf("\t Test \n");
        }
    
	return 1 ;
}
int	ScriptsManager::UnloadScript (string script_file)
{
    YDLE_DEBUG <<  script_file  <<std::endl;
    string script = PARAM_STR("lua-scripts.scripts_dir")+ "/"+ script_file + ".lua" ;
    for( Kernel::FeatureList::iterator it = pFeatures->begin(); it != pFeatures->end(); ++it) {
		IFeature * feature = *it ;
        //        IFeature  feature = "lua-script";
                //feature->AddScript (script.c_str()) ;
                feature->UnloadScript(script.c_str());
                printf ("\tFeature : %s:UnloadScript \n", feature->Name().c_str()) ;
        //intf("\t Test \n");
        }
    
	return 1 ;
}