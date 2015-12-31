/*
 * IFeature.h
 *
 *  Created on: Fev 24, 2014
 *      Author: fets
 */

#ifndef IFEATURE_H
#define IFEATURE_H

#include <string>

#include "Ident.h"

namespace ydleMaster {

class IFeature : public Ident
{
  public:
    //virtual std::string Name ()  = 0 ;
    virtual void Init ()  = 0 ;
    virtual void Start ()  = 0 ;

    virtual void AddScript( const char * name) = 0 ;
    virtual void ReloadScript(const char * name) = 0;
    virtual void LoadScript(const char * name) = 0;
    virtual void UnloadScript(const char * name) = 0;

  public:
    IFeature (std::string name) : Ident(name) {} ;
    ~IFeature () {} ;
} ;

} ; // namespace ydleMaster

#endif // IFEATURE_H