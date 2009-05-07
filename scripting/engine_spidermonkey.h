// engine_spidermonkey.h

#pragma once


#include "engine.h"

#ifdef _WIN32
#define XP_WIN
#else
#define XP_UNIX
#endif

#ifdef MOZJS
#include "mozjs/jsapi.h"
#include "mozjs/jsdate.h"
#else
#include "js/jsapi.h"
#include "js/jsdate.h"
#endif

namespace mongo {

    class SMScope;
    extern JSClass bson_ro_class;
    extern JSClass object_id_class;

    // internal things
    void dontDeleteScope( SMScope * s ){}
    void errorReporter( JSContext *cx, const char *message, JSErrorReport *report );
    extern boost::thread_specific_ptr<SMScope> currentScope;
    
    // bson
    JSBool resolveBSONField( JSContext *cx, JSObject *obj, jsval id, uintN flags, JSObject **objp );


    // mongo
    void initMongoJS( SMScope * scope , JSContext * cx , JSObject * global , bool local );
    
}