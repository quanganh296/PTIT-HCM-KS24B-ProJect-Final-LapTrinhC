	#ifndef DATATYPE_H
	#define DATATYPE_H
	#include <stdbool.h>
	#include "function.h"  
	

	
	typedef struct {
	    int day;
	    int month;
	    int year;
	} Date;
	
	//struct Transaction{
	//	char transferId[10];
	//	char receivingId[10];
	//	double amount;
	//	char type[10];
	//	char message[50];
	//	struct Date transactionDate;
	//};
	//
	//struct AccountInfo{
	//	char userId[20];
	//	float balance;
	//	bool status;
	//	char username[10];
	//	char password[10];
	//	struct Transaction transactionHistory[];
	//};
	
	
	typedef struct {
	    char userId[20];
	    char name[20];
	    Date dateOfBirth;
	    char phone[15];
	    char email[30];
	      int status;
	      char username[20];
		    char password[20];
		    
	} User;
		extern User users[];
	#endif
	
