#include <stdio.h>
#include <time.h>
#include "mysql.h"

int main(int argc, char **argv)
{

    double time_s = 0.0;

    clock_t begin= clock();
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if (mysql_real_connect(con, "localhost", "root", "Hailsatan666",
                           NULL, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    if (mysql_query(con, "USE testdb"))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    if (mysql_query(con, "DROP DATABASE testdb"))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
    if (mysql_query(con, "CREATE DATABASE IF NOT EXISTS testdb"))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
    if (mysql_query(con, "USE testdb"))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
    if (mysql_query(con, "CREATE TABLE IF NOT EXISTS Persons (PersonID int,LastName varchar(255),FirstName varchar(255),Address varchar(255),City varchar(255))"))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }
    if (mysql_query(con, "ALTER TABLE Persons ADD Email varchar(255);"))
    {
        fprintf(stderr, "%s\n", mysql_error(con));
        mysql_close(con);
        exit(1);
    }

    mysql_close(con);

    clock_t end=clock();

    time_s += (double)(end - begin) / CLOCKS_PER_SEC;

	printf("Time elpased is %f seconds \n", time_s);

    exit(0);
}