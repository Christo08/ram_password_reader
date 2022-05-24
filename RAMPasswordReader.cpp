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
    string pathToFile;
    //pathToFile = = "C:\\Users\\User\\Downloads\\after_close.img";

    cout<<"Please enter the path to the bump file:"<<endl;
    cin>>pathToFile;
    pathToFile = "C:\\Users\\User\\Downloads\\execel\\after_auth.img";

    cout<<"Making text for bump."<<endl;
    ifstream MyReadFile("bump.txt");
    string lines ="";
    string text ="";

    string commad = "strings "+pathToFile+" > bump.txt";

    ofstream {"bump.txt"};

    exec(commad.c_str());

    while (getline (MyReadFile, lines)) {
        text += lines + '\n';
    } 

    cout<<"Please select 1 for a zip file, 2 for a xlsx file and 3 for a ftp client."<<endl;
    int passwordType;
    cin >> passwordType;

    cout<<"Searching for password."<<endl;

    if (passwordType == 1)
    {
        std::regex patternBlock(" Directory \\d+ for .+\\.zip.+\\..+\\n\\|.+\\n(:=B~(p?))");
        smatch matches; 
        
        if(regex_search(text, matches, patternBlock))
        {
            string password = matches.str().substr(matches.str().find('|')+1);
            password= password.substr(0,password.find('\n'));
            cout<<"Password fond: "<<password<<endl;
        }
        else
        {
            cout<<"Password not fond."<<endl;
        }
    }
    else if(passwordType == 2)
    {

    }
    else if(passwordType == 3)
    {
        std::regex patternBlock1("Bookshelf Symbol 7\\n.+\\nreateTab");
        std::regex patternBlock2("\\<User\\>.+\\</User\\>.*\\n.*\\<Pass\\>.+\\</Pass\\>.*\\n.*\\<Logontype\\>\\d\\</Logontype\\>");
        smatch matches; 
        
        if(regex_search(text, matches, patternBlock1))
        {
            string block = matches.str();
            
            vector<string> linesOfBlock;
 
            stringstream ss(block);
            string lineOfBlock;
            while (getline(ss, lineOfBlock, '\n')) {
                linesOfBlock.push_back(lineOfBlock);
            }
            cout<<"Password fond: "<<linesOfBlock[1]<<endl;
        }
        else if(regex_search(text, matches, patternBlock2))
        {
            string block = matches.str();
            vector<string> linesOfBlock;
 
            stringstream ss(block);
            string lineOfBlock;
            while (getline(ss, lineOfBlock, '\n')) {
                linesOfBlock.push_back(lineOfBlock);
            }

            string password = linesOfBlock[1];
            password = password.substr(password.find('>')+1);
            password = password.substr(0, password.find('<'));
            cout<<"Password fond: "<<password<<endl;
        }
        else
        {
            cout<<"Password not fond."<<endl;
        }

    }
    
}

void getFootprint()
{
    string pathToFile;
    //"C:\Users\User\Downloads\after_close.img  zip_17094446_pass";

    cout<<"Please enter the path to the bump file:"<<endl;
    cin>>pathToFile;

    cout<<"Laoding text"<<endl;
    ifstream MyReadFile("bump.txt");
    string line ="";
    string text ="";

    string commad = "strings "+pathToFile+" > bump.txt";

    ofstream {"bump.txt"};

    exec(commad.c_str());

    cout<<"Search for footprint."<<endl;

    string password;
    cout<<"Please enter the password:"<<endl;
    cin>>password;

    string lines[11] = {"","","","","","","","","","",""};

    bool findPassword =false;

    while (getline (MyReadFile, line)) {
        for (int counter = 1; counter < 11; counter++)
        {
            lines[counter-1]=lines[counter];
        }
        lines[10] = line;
        if (lines[5].find(password)!=string::npos)
        {
            findPassword = true;
           break;
        }        
    }
    
    if (findPassword)
    {
        cout<<"Footprint before password:"<<endl;
        for (int counter = 0; counter < 5; counter++)
        {
            cout<<(counter+1)<<' '<<lines[counter]<<endl;
        }
        int startPosition = lines[5].find(password);
        cout<<"6 "<<lines[5].substr(0, startPosition)<<endl;
        cout<<"Footprint after password:"<<endl;
        cout<<"1 "<<lines[5].substr((startPosition + password.length()))<<endl;
        for (int counter = 6; counter < 11; counter++)
        {
            cout<<(counter+2-6)<<' '<<lines[counter]<<endl;
        }
    }
    else
    {
        cout<<"Footprint not fond."<<endl;
    }
    

}

int main() 
{ 
    int mode;
    do{
        cout<<"Press 1 to get the password, 2 to get the footprint and 3 to quit the program."<<endl;
        cin>>mode;
        if (mode == 1)
        {
            getPassword(); 
        }
        else if (mode == 2)
        {
            getFootprint();
        }   
         
    }while(mode != 3);
    cout<<"Thank for using the program. Bye."<<endl;
    return 0; 
}