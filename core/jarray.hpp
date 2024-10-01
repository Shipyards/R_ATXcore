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

#ifdef R_JATXCORE_EXPORTS
#pragma message("MATHLIBEXPT is defined in that one annyoing file")
#define R_JATXcore_API __declspec(dllexport)
#else
#define R_JATXcore_API __declspec(dllimport)
#endif

#pragma once

#include <mutex>
#include "pch.h"
#include "framework.h"

namespace JATX
{
	template<typename containertype>
	class R_jarray // robust storage class
	{
	private:
		containertype* internalarray;
		int size;
	public:
		R_jarray(int newsize) : size(newsize)
		{
			this->internalarray = new containertype[newsize];
		}
		R_jarray() : size(1)
		{
			this->internalarray = new containertype[1];
		}
		~R_jarray()
		{
			delete[] this->internalarray;
		}
		containertype& operator[](int access)
		{
			//if (access >= this->size - 1) { return 0; }
			//else if (this->internalarray[access] == nullptr) { return 0; }
			return this->internalarray[access];
		}
		int _length() { return this->size; }
	};
	template<typename containertype>
	class R_jarray<containertype*> // robust storage class (pointer specialization), !will delete all pointers in array!
	{
	private:
		containertype** internalarray;
		int size;
	public:
		R_jarray(int newsize) : size(newsize)
		{
			this->internalarray = new containertype * [newsize];
			for (int i = 0; i != newsize; i++)
			{
				this->internalarray[i] = nullptr;
			}
		}
		~R_jarray()
		{
			for (int i = 0; i != this->size; i++)
			{
				if (this->internalarray[i] == nullptr) { continue; }
				delete this->internalarray[i];
			}
			delete[] this->internalarray;
		}
		containertype*& operator[](int access)
		{
			/*if (access >= this->size - 1) { return NULL; }
			else if (this->internalarray[access] == nullptr) { return NULL; }*/
			return this->internalarray[access];
		}
		int _length() { return this->size; }
	};
	

	//thread safe robust array
	template<typename containertype>
	class TS_R_jarray :
		public R_jarray<containertype>
	{
	private:
		containertype* internalarray;
		std::mutex internalmtx;
		int size;
	public:
		TS_R_jarray(int newsize) : size(newsize)
		{
			this->internalmtx.lock();
			this->internalarray = new containertype[newsize];
			this->internalmtx.unlock();
		}
		~TS_R_jarray()
		{
			this->internalmtx.lock();
			delete[] this->internalarray;
			this->internalmtx.unlock();
		}
		void _lock() { this->internalmtx.lock(); }
		void _unlock() { this->internalmtx.unlock(); }
		containertype read(int access) //returns copy of container
		{ 
			containertype placeholder;

			//lock while copying the object
			this->_lock();
			//copy object
			placeholder = this->internalarray[access];
			//unlock once object is copied
			this->_unlock();

			return placeholder;
		}
		containertype& fetch(int access) //returns address of container
		{
			this->_lock();

			return this->internalarray[access];
		}
		bool write(int access, containertype writethis) //writes container
		{ 
			//lock while writing
			this->internalmtx.lock();
			//write
			this->internalarray[access] = writethis;
			//release lock after writing
			this->_unlock();

			return true; 
		}
		//containertype& operator[](int access)
		//{
		//	/*if (access >= this->size - 1) { throw; }
		//	else if (this->internalarray[access] == nullptr) { throw; }*/

		//	containertype& placeholder;

		//	//lock why copying the object
		//	this->internalmtx.lock();
		//	//copy object
		//	placeholder = this->internalarray[access];
		//	//unlock once object is copied
		//	this->internalmtx.unlock();

		//	return placeholder;
		//}
		int _length() { return this->size; }
	};
	template<typename containertype>
	class TS_R_jarray<containertype*> // thread safe robust array (pointer specialization), !will delete pointers!
	{
	private:
		containertype** internalarray;
		std::mutex internalmtx;
		int size;
	public:
		TS_R_jarray(int newsize) : size(newsize)
		{
			this->internalmtx.lock();
			this->internalarray = new containertype * [newsize];
			for (int i = 0; i != newsize - 1; i++)
			{
				this->internalarray[i] = nullptr;
			}
			this->internalmtx.unlock();
		}
		~TS_R_jarray()
		{
			this->internalmtx.lock();
			for (int i = 0; i != this->size - 1; i++)
			{
				if (this->internalarray[i] == nullptr) { continue; }
				delete this->internalarray[i];
			}
			delete[] this->internalarray;
			this->internalmtx.unlock();
		}
		void _lock() { this->internalmtx.lock(); }
		void _unlock() { this->internalmtx.unlock(); }
		containertype* read(int access) //returns copy of container
		{ 
			containertype* placeholder;

			//aquire lock while copying adress
			this->_lock();
			//copy adress
			placeholder = this->internalarray[access];
			//release lock once adress is copied
			this->_unlock();

			return placeholder;
		}
		containertype*& fetch(int access) //returns address of container
		{
			this->_lock();

			return this->internalarray[access];
		}
		bool write(int access, containertype* writethis) 
		{ 
			//lock while writing
			this->_lock();
			//write
			this->internalarray[access] = writethis; 
			//unlock after writing
			this->_unlock();

			return true; 
		}
		//containertype*& operator[](int access)
		//{
		//	/*if (access >= this->size - 1) { return NULL; }
		//	else if (this->internalarray[access] == nullptr) { return NULL; }*/

		//	containertype*& placeholder;

		//	//aquire lock while copying adress
		//	this->internalmtx.lock();

		//	//copy adress
		//	placeholder = this->internalarray[access];
		//	//release lock once adress is copied
		//	this->internalmtx.unlock();

		//	return placeholder;
		//}
		int _length() { return this->size; }
	};

	/*template<typename arrtype>
	extern R_JATXcore_API R_jarray<arrtype>* new_R_jarray(int);

	template<typename arrtype>
	extern R_JATXcore_API TS_R_jarray<arrtype>* new_TS_R_jarray(int);*/
}