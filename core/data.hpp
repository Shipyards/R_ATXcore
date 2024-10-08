/*
   Copyright 2024 Jacob T. Ray

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

//#ifdef MATHLIBRARY_EXPORTS
//#define R_JATXcore_API __declspec(dllexport)
//#else
//#define R_JATXcore_API __declspec(dllimport)
//#endif

#pragma once

#include <iostream>
#include "baseContainer.hpp"
#include "UID.h"

namespace JATX
{
    // data storage class [virtual]
    /*R_JATXcore_API*/ class _data :
        public _baseContainer
    {
    public:
        UID localUID;
        //constructor
        _data() { this->localUID = *(new UID()); };
        // get raw data
        virtual char* _raw() = 0;
        //serialize
        virtual char* _serialize() = 0;
        //deserialize
        virtual bool _deserialize(char*) = 0;
        //deconstructor
        ~_data() {};
    };
}