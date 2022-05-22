#include <iostream> 
#include <regex> 
#include <string.h> 
#include <fstream>
using namespace std; 
   

string exec(const char* cmd) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) 
    {
        throw runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) 
    {
        result += buffer.data();
    }
    return result;
}

void getPassword()
{
    cout<<"Laoding text"<<endl;
    ifstream MyReadFile("bump.txt");
    string lines ="";
    string text ="";

    string pathToFile = "C:\\Users\\User\\Downloads\\after_close.img";

    string commad = "strings "+pathToFile+" > bump.txt";

    ofstream {"bump.txt"};

    exec(commad.c_str());

    std::regex patternBlock(" Directory \\d+ for .+\\.zip.+\\..+\\n\\|.+\\n(:=B~(p?))");
    smatch matches; 

    while (getline (MyReadFile, lines)) {
        text += lines + '\n';
    } 
    cout<<"Search for password "<<endl;
    
    if(regex_search(text, matches, patternBlock))
    {
        string password = matches.str().substr(matches.str().find('|')+1);
        password= password.substr(0,password.find('\n'));
        cout<<"Password fond "<<password<<endl;
    }
    else
    {
        cout<<"Password not fond"<<endl;
    }
}

int main() 
{ 
    getPassword();  
    return 0; 
}