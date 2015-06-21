/*osm2pgRouting

Copyright (c) 2015 Sarthak Agarwal
sarthak0415@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

/*to compile and run
>>  g++ prog_options.cpp -o a -lboost_program_options -g -O3 -std=c++0x -Wall -pedantic
>> ./a --help
*/


#include <boost/config.hpp>
#include <string>
#include <boost/program_options.hpp>
namespace po = boost::program_options;


#include <iostream>
#include <fstream>
#include <iterator>
using namespace std;

// A helper function to simplify the main part.
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(os, " ")); 
    return os;
}

int main(int ac, char* av[])
{
    try {
        
        //Declare a group of options that will be 
        // allowed only on command line

        //cmdline_options have options that can be given on command line
        //config_file_options have options that can be given through config file only
        //od_desc has all the commands

        po::options_description od_desc("Allowed options");
        po::options_description cmdline_options;
        po::options_description config_file_options;

        string config_file; 
        string filename ;
        po::options_description generic("Generic options");
        generic.add_options()
            ("file,f",po::value<string>(&filename),"filename")
            ("conf,c",po::value<string>(&config_file)->default_value("prog_options.cfg"),"configuration file path/name (ask vicky)")
            ("help", "produce help message")
            ;

        // Declare a group of options that will be 
        // allowed both on command line and in
        // config file
        string dbname,username,host,port,password;
        po::options_description config("Configuration");
        config.add_options()
            ("dbname,d",po::value<string>(&dbname),"database name to be imported")
            ("username,u",po::value<string>(&username),"name of the username of the database")
            ("host,h",po::value<string>(&host),"host name, eg: localhost(127.0.0.1)")
            ("port,p",po::value<string>(&port),"port name, eg: 8080")
            ("password,passwd",po::value<string>(&password),"password")
            ;

        //Dont need it as of now
        /* 
        // Hidden options, will be allowed both on command line and
        // in config file, but will not be shown to the user.
        po::options_description hidden("Hidden options");
        hidden.add_options()
            ("input-file", po::value< vector<string> >(), "input file")
            ;
        */
        
        cmdline_options.add(generic).add(config);

        config_file_options.add(config);

        od_desc.add(generic).add(config);
        
        
        //Do we need a positional option for filename/dbname??
        /*
        po::positional_options_description p;
        p.add("input-file", -1);
        */

        po::variables_map vm;
        store(po::command_line_parser(ac, av).
              options(cmdline_options).run(), vm);
        notify(vm);

        printf("config file is %s \n",config_file.c_str());
        
        ifstream ifs(config_file.c_str());
        if (!ifs)
        {
            cout << "can not open config file: " << config_file << "\n";
            return 0;
        }
        else
        {
            store(parse_config_file(ifs, config_file_options), vm);
            notify(vm);
        }
        
        if (vm.count("help")) {
            cout << od_desc << "\n";
            return 0;
        }

        if (vm.count("file"))
            cout << "Filename is: " << vm["file"].as<string>() << "\n";
        else
            std::cout << "Parameter: file missing\n";
        
        if (vm.count("conf"))
            cout << "configuration filename is: "<< vm["conf"].as<string>() << "\n";
        else
            std::cout << "Parameter: config file name missing\n";
        
        if (vm.count("dbname")) 
            std::cout << "dbname = " << vm["dbname"].as<std::string>() << "\n";
        else
            std::cout << "Parameter: dbname missing\n";

        if (vm.count("host")) 
            std::cout << "host = " << vm["host"].as<std::string>() << "\n";
        else
            std::cout << "Parameter: host missing\n";

        if (vm.count("port")) 
            std::cout << "port = " << vm["port"].as<std::string>() << "\n";
        else
            std::cout << "Parameter: port missing\n";

        if (vm.count("username")) 
            std::cout << "username = " << vm["username"].as<std::string>() << "\n";
        else
            std::cout << "Parameter: username missing\n";

        if (vm.count("password"))
            cout << "password is: " << vm["password"].as<string>() << "\n";
        else
            std::cout << "Parameter: password missing\n";        
        

        if (vm.count("dbname") & vm.count("username") & vm.count("host") & vm.count("password") ) {
            std::cout << "Parameters: \n"
                 << vm["dbname"].as<std::string>() << "\n"
                 << vm["username"].as<std::string>() << "\n"
                 << vm["host"].as<std::string>() << "\n"
                 << vm["password"].as<std::string>() << ".\n";

            return 2;
        } else {
            std::cout << "Missing parameter.\n";
            std::cout << od_desc << "\n";
            return 1;
        }

        
    }
    catch(exception& e)
    {
        cout << e.what() << "\n";
        return 1;
    }    
    return 0;
}
