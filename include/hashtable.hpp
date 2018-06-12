#ifndef _HASH_HPP_
#define _HASH_HPP_

#include <iostream>
#include <iterator>
#include <forward_list>
#include <vector>
#include <math.h>
#include <string>

/** 
 *	\file 	hash.hpp
 * 	\author	Felipe Ramos
 * 	\title	Hash-Table project
 */

using size_type = size_t; 	// Simple Alias

/** Hash Table Class */
template <
			class KeyType,
		  	class DataType,
		  	class KeyHash = std::hash<KeyType>,
		  	class KeyEqual = std::equal_to<KeyType>
		  >
class HashTbl{
	/** Hash Entry Class, used to hold keys/data from the hash table. */
	class HashEntry{
	/*{{{*/
		public:
			/** HashEntry Constructor */
			HashEntry(KeyType k_, DataType d_): m_key(k_), m_data(d_){};

			KeyType m_key;		//!< Stores the key for an entry.
			DataType m_data;	//!< Stores the data for an entry.
	};
	/*}}}*/

	public:
	/*{{{*/
		using Entry = HashEntry;

		/** Hash Table Constructor */
		HashTbl( void ): tablesize(101), currentSize(0){
			Lists.resize(101);
		}

		/** Table constructor */
		explicit HashTbl( size_type size ):
			tablesize( next_prime(size) ), currentSize(0){
					Lists.resize( next_prime(size) );
			}

		/** Inserts Elements related to a `key_` on the table.
		 *	\return True if the insertion was successful, false otherwise. */ 
		bool insert( const KeyType &, const DataType & );

		/** Recovers the information related to the `key_`.
		 *	\return True if it finds the information, False otherwise. */
		bool retrieve( const KeyType &,  DataType & ) const;

		/** Prints the entire hash table. */
		void print() const;

		/** Clear the lists related memories. */
		void clear();
		
		/** Tells if a HashTbl is empty. */
		bool empty() const;

		/** Tells the current total elements are in a list. */
		size_type count() const;

		/** Tells how many elements a table supports. */
		size_type capacity() const;

		/** Removes an specific element `key_`. 
		 * \return True if the `key_` was found, false otherwise */
		bool remove( const KeyType & );
	/*}}}*/

	private:
	/*{{{*/
		/* Vector with the list of elements (for collisions) */
		std::vector< std::forward_list<Entry> > Lists;
		
		size_type tablesize; 		//!< Hash table current size
		size_type currentSize; 		//!< Total of elements in the table
	
		/* Auxiliary Funcs */
		KeyHash hashFunc; 
		KeyEqual equalFunc;

		/** Helps finding the next prime number after table size. */
		size_type next_prime( size_type );
		
		/** Resizes the Hash Table. */
		void rehash();
	/*}}}*/
};

// Source Implementation

/* Public Implement {{{*/
template < class KeyType, class DataType, class KeyHash, class KeyEqual	>
bool HashTbl<KeyType, DataType, KeyHash, KeyEqual>::insert
( const KeyType &key_, const DataType &data_item_ ){
/*{{{*/
	/* Elements list */	
	auto & whichList = Lists[ hashFunc( key_ ) % tablesize ];
	Entry new_entry( key_, data_item_ );
	auto itr = whichList.begin(); 							//!< Iterator to the List's begin
	auto itr_b = whichList.before_begin(); 					//!< Iterator to the position before the list's begin 
	auto end = whichList.end(); 							//!< Iterator to the list's end
	for ( /* */; itr != end; ++itr ){
		itr_b++;
		if ( equalFunc((*itr).m_key, new_entry.m_key) )
			return false;
	}

	whichList.insert_after( itr_b, new_entry );

	if( ++currentSize > tablesize ) 
		rehash();

	return true; 		// If operation was successful
}
/*}}}*/

template < class KeyType, class DataType, class KeyHash, class KeyEqual	>
bool HashTbl<KeyType, DataType, KeyHash, KeyEqual>::retrieve
( const KeyType &key_, DataType &data_item_ ) const{
/*{{{*/
	/* Elements list */
	auto & whichList = Lists[ hashFunc(key_) % tablesize ];
	for( auto i = whichList.begin(); i != whichList.end(); ++i ){
		if ( equalFunc((*i).m_key , key_) ){
			data_item_ = (*i).m_data;
			return true;
		}
	}
	return false;
}
/*}}}*/

template < class KeyType, class DataType, class KeyHash, class KeyEqual	>
void HashTbl<KeyType, DataType, KeyHash, KeyEqual>::print( void ) const{
/*{{{*/
	if( !Lists.empty() ){
		for( auto Lists_iter = Lists.begin(); Lists_iter != Lists.end(); ++Lists_iter ){
			if( !(*Lists_iter ).empty() )
				for( auto j = (*Lists_iter).begin(); j != (*Lists_iter).end(); ++j ){
					// std::cout << "\n\nKey { ";
					// std::cout << (*j).m_key;
					// std::cout << " };\nData {";
					std::cout << (*j).m_data;
					// std::cout << "};\n";
				}
		}

	}
}
/*}}}*/

template < class KeyType, class DataType, class KeyHash, class KeyEqual	>
void HashTbl<KeyType, DataType, KeyHash, KeyEqual>::clear( void ){
/*{{{*/
	Lists.clear();
}
/*}}}*/

template < class KeyType, class DataType, class KeyHash, class KeyEqual	>
bool HashTbl<KeyType, DataType, KeyHash, KeyEqual>::empty( void ) const{
/*{{{*/
	return this->currentSize == 0;
}
/*}}}*/

template < class KeyType, class DataType, class KeyHash, class KeyEqual	>
size_type HashTbl<KeyType, DataType, KeyHash, KeyEqual>::count( void ) const{
/*{{{*/
	return this->currentSize;
}
/*}}}*/

template < class KeyType, class DataType, class KeyHash, class KeyEqual	>
size_type HashTbl<KeyType, DataType, KeyHash, KeyEqual>::capacity( void ) const{
/*{{{*/
	return this->tablesize;
}
/*}}}*/

template < class KeyType, class DataType, class KeyHash, class KeyEqual	>
bool HashTbl<KeyType, DataType, KeyHash, KeyEqual>::remove( const KeyType &key_ ){
/*{{{*/
	auto &whichList = Lists[ hashFunc(key_) % tablesize ];
	auto itr_back = whichList.before_begin();
	for( auto i = whichList.begin(); i != whichList.end(); ++i ){
		if( equalFunc((*i).m_key, key_ ) ){
			whichList.erase_after( itr_back );
			--currentSize;
			return true;
		}
		itr_back++;
	}	
	return false;
}
/*}}}*/

/*}}}*/

/* Private Implement {{{*/

template < class KeyType, class DataType, class KeyHash, class KeyEqual	>
size_type HashTbl<KeyType, DataType, KeyHash, KeyEqual>::next_prime( size_type number ){
/*{{{*/
	for( size_type i = 2; i < sqrt( number ); ++i ) {
		if(number % i == 0) return next_prime( number + 1 );
	}
	return number;
}
/*}}}*/

template < class KeyType, class DataType, class KeyHash, class KeyEqual	>
void HashTbl<KeyType, DataType, KeyHash, KeyEqual>::rehash( void ){
/*{{{*/
	std::vector< std::forward_list<Entry> > oldLists = Lists;
	
	/* Creates a new double-sized empty table */
	tablesize = next_prime( tablesize * 2 );
	
	Lists.resize( tablesize );
	currentSize = 0;

	for( auto & thisList : Lists )
		thisList.clear(); 

	/* Copies table over */
	for( auto & copyLists : oldLists )
		for( auto & x : copyLists )
			insert( x.m_key, x.m_data );
}
/*}}}*/

/*}}}*/

#endif
