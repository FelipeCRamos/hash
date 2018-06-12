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
			class kType,
		  	class dType,
		  	class kHash = std::hash<kType>,
		  	class kEqual = std::equal_to<kType>
		  >
class HashTbl{
	/** Hash Entry Class, used to hold keys/data from the hash table. */
	class HashEntry{
	/*{{{*/
		public:
			/** HashEntry Constructor */
			HashEntry(kType k_, dType d_): m_key(k_), m_data(d_){};

			kType m_key;		//!< Stores the key for an entry.
			dType m_data;	//!< Stores the data for an entry.
	};
	/*}}}*/

	public:
	/*{{{*/
		typedef HashEntry Entry;

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
		bool insert( const kType &, const dType & );

		/** Recovers the information related to the `key_`.
		 *	\return True if it finds the information, False otherwise. */
		bool retrieve( const kType &,  dType & ) const;

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
		bool remove( const kType & );
	/*}}}*/

	private:
	/*{{{*/
		typedef std::forward_list<Entry> _cList;	// Colision list holder

		/* Vector with the list of elements (for collisions) */
		std::vector<_cList> Lists;
		
		size_type tablesize; 		//!< Hash table current size
		size_type currentSize; 		//!< Total of elements in the table
	
		/* Auxiliary Funcs */
		kHash _hFunc; 
		kEqual eFunc;

		/** Helps finding the next prime number after table size. */
		size_type next_prime( size_type );
		
		/** Resizes the Hash Table. */
		void rehash();
	/*}}}*/
};

// Source Implementation

/* Public Implement {{{*/
template < class kType, class dType, class kHash, class kEqual	>
bool HashTbl<kType, dType, kHash, kEqual>::insert
( const kType &k_, const dType &d_item ){
/*{{{*/
	/* Elements list */	
	auto & whichList = Lists[ _hFunc( k_ ) % tablesize ];
	Entry new_entry( k_, d_item );
	auto itr = whichList.begin(); 							//!< Iterator to the List's begin
	auto itr_b = whichList.before_begin(); 					//!< Iterator to the position before the list's begin 
	auto end = whichList.end(); 							//!< Iterator to the list's end
	for ( /* */; itr != end; ++itr ){
		itr_b++;
		if ( eFunc((*itr).m_key, new_entry.m_key) )
			return false;
	}

	whichList.insert_after( itr_b, new_entry );

	if( ++currentSize > tablesize ) 
		rehash();

	return true; 		// If operation was successful
}
/*}}}*/

template < class kType, class dType, class kHash, class kEqual	>
bool HashTbl<kType, dType, kHash, kEqual>::retrieve
( const kType &key_, dType &d_item ) const{
/*{{{*/
	/* Elements list */
	auto & whichList = Lists[ _hFunc(key_) % tablesize ];
	for( auto i = whichList.begin(); i != whichList.end(); ++i ){
		if ( eFunc((*i).m_key , key_) ){
			d_item = (*i).m_data;
			return true;
		}
	}
	return false;
}
/*}}}*/

template < class kType, class dType, class kHash, class kEqual	>
void HashTbl<kType, dType, kHash, kEqual>::print( void ) const{
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

template < class kType, class dType, class kHash, class kEqual	>
void HashTbl<kType, dType, kHash, kEqual>::clear( void ){
/*{{{*/
	Lists.clear();
}
/*}}}*/

template < class kType, class dType, class kHash, class kEqual	>
bool HashTbl<kType, dType, kHash, kEqual>::empty( void ) const{
/*{{{*/
	return this->currentSize == 0;
}
/*}}}*/

template < class kType, class dType, class kHash, class kEqual	>
size_type HashTbl<kType, dType, kHash, kEqual>::count( void ) const{
/*{{{*/
	return this->currentSize;
}
/*}}}*/

template < class kType, class dType, class kHash, class kEqual	>
size_type HashTbl<kType, dType, kHash, kEqual>::capacity( void ) const{
/*{{{*/
	return this->tablesize;
}
/*}}}*/

template < class kType, class dType, class kHash, class kEqual	>
bool HashTbl<kType, dType, kHash, kEqual>::remove( const kType &key_ ){
/*{{{*/
	auto &whichList = Lists[ _hFunc(key_) % tablesize ];
	auto itr_back = whichList.before_begin();
	for( auto i = whichList.begin(); i != whichList.end(); ++i ){
		if( eFunc((*i).m_key, key_ ) ){
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

template < class kType, class dType, class kHash, class kEqual	>
size_type HashTbl<kType, dType, kHash, kEqual>::next_prime( size_type number ){
/*{{{*/
	for( size_type i = 2; i < sqrt( number ); ++i ) {
		if(number % i == 0) return next_prime( number + 1 );
	}
	return number;
}
/*}}}*/

template < class kType, class dType, class kHash, class kEqual	>
void HashTbl<kType, dType, kHash, kEqual>::rehash( void ){
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
