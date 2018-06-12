#include <iostream>
#include <functional> // EXIT_SUCCESS
#include <cassert> // assert
#include <tuple>

#include "hashtable.hpp"

/** Accounts definition */
struct Account{
    typedef int AcctKey;
    std::string client_name;
    int bank_code;
    int branch_code;
    int acc_number;
    float balance;

    Account(
		std::string n_ = "",
		int bnc_ = 1,
		int brc_ = 1999,
		int nmr_ = 0,
		float bal_ = 0.f
	):	client_name( n_ ),
        bank_code( bnc_ ),
        branch_code( brc_ ),
        acc_number( nmr_ ),
        balance( bal_ )
    { /* Empty */ }

	/* Public */        
    
	/** Returns the key related to the account. */
    AcctKey get_key() { return acc_number; }

	/** Operator overflow for printing things. */
    inline friend std::ostream &operator<<
		( std::ostream & _os, const Account & _acc ){
        _os << "\n"
			"\tClient: \t'" << _acc.client_name 	<< "';\n"
            "\tBank:   \t" << _acc.bank_code 	<< ";\n"
            "\tBranch: \t" << _acc.branch_code 	<< ";\n"
            "\tNumber: \t" << _acc.acc_number 	<< ";\n"
            "\tBalance:\t" << _acc.balance 		<< ";\n";
        return _os;
    }
};

/* Hash function */
struct KeyHash{
    std::size_t operator()( const Account::AcctKey & k_ ) const{
        std::cout << "\n\t>>> KeyHash() key = ";
		std::cout << std::hash< int >()( k_ ) << "\n";
        
		/* Calculates a hash value */
        return std::hash< int >()( k_ );
    }
};

/* Comparation function */
struct KeyEqual{
	bool operator()
		( const Account::AcctKey & _lhs, const Account::AcctKey & _rhs ) const{
		return ( _lhs == _rhs );
	}
};

int main(){
    {
        //REMAKE
		/* REMAKE */
    	Account acct("John Doe", 1, 1999, 44876, 10000.f);

    	Account _accounts[] =
            {
                {"Young-Ho Jannah", 	1, 	1999, 	44876, 	10000.f	},
                {"Abel Ole", 			1, 	1999, 	45794, 	0.f		},
                {"Servaas Feardorcha", 	2, 	557, 	87629, 	1050.f	},
                {"Seo-Yun Sat", 		12,	331, 	1231, 	850.f	},
                {"Anitta", 				4, 	666,	5,		53021.f	}
            };

    	HashTbl< Account::AcctKey, Account > contas(23);

        assert( contas.capacity() == 23);
        assert( contas.count() == 0 );

		for( int i = 0; i < 5; i++ )
			contas.insert( _accounts[i].get_key(), _accounts[i] );

        // Printing Accounts
        std::cout << ">> Current accounts:\n";
        contas.print();
        assert( contas.count() == 5 );

		std::cout << "STATUS: Trying to delete " 
			<< _accounts[1].client_name << " account...\n";
        if( contas.remove( _accounts[1].get_key() ) )
            std::cout << "STATUS: Removed successfully\n";
        else
            std::cout << "ERROR: Account not found!\n";

        contas.print();
        assert( contas.count() == 4 );

        Account acc1;
        contas.retrieve( _accounts[3].get_key(),  acc1);
        assert( _accounts[3].get_key() ==  acc1.get_key() );
        assert( _accounts[3].client_name ==  acc1.client_name );
        assert( _accounts[3].branch_code ==  acc1.branch_code );
        assert( !contas.retrieve( _accounts[1].get_key(),  acc1) ); 
    }

	/* Rehash Testing */ 
    {
        std::cout << ">> Testing Rehash()...\n";
        Account accs[] = {
                {"Paulo", 	1, 	1999, 	35412, 	10000.f	},
                {"Amanda", 	1, 	1997, 	63526, 	530.f	},
                {"Felipe", 	2, 	1998, 	32133, 	100000.f},
                {"Max", 	3, 	102, 	27840, 	850.f	},
                {"Oziel", 	2, 	666, 	98766, 	5490.f	},
                {"Daniel", 	5, 	128, 	99999, 	5490.f	}
            };

        std::cout << ">> Creating a Hash Table with this->capacity = 2...\n";

        HashTbl< Account::AcctKey, Account > contas(2);

        assert( contas.capacity() == 2 );
        assert( contas.count() == 0 );

        std::cout << ">> DONE!\n";

        // Adds more accounts than it supports, then, it will need to rehash
		for( int i = 0; i < 6; i++ )
			contas.insert(accs[i].get_key(), accs[i]);

        contas.print();

        std::cout << ">> Rehash worked Successfully!\n";

        assert( contas.count() == 6 );
    }

    std::cout << "\n\n>> Exiting Successfully...\n";
	return EXIT_SUCCESS;
}
