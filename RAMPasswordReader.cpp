#include <iostream> 
#include <filesystem>
#include <regex> 
#include <string.h> 
#include <fstream>
using namespace std; 
   

string exec(const char* cmd) {
    //Execute a commad and return the output
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
    //Get path to dump file
    string pathToFile;
    cout<<"Please enter the path to the dump file:"<<endl;
    cin>>pathToFile;

    cout<<"Making text for dump."<<endl;

    string commad = "strings "+pathToFile+" > dumpText.txt";

    //Create text file
    ofstream {"dumpText.txt"};
    
    //Create file reader
    ifstream MyReadFile("dumpText.txt");
    string lines ="";
    string text ="";

    //Execute commad to change dump file to text
    exec(commad.c_str());

    //Get text for the file
    while (getline (MyReadFile, lines)) {
        text += lines + '\n';
    } 
    
    //Get type of file
    int passwordType;
    do{
        cout<<"Please select 1 for a zip file, 2 for a xlsx file and 3 for a ftp client."<<endl;
        cin >> passwordType;
    }while(passwordType != 1 && passwordType != 2 && passwordType != 3);

    cout<<"Searching for password."<<endl;

    if (passwordType == 1)
    {
        //Seacrh for zip password
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
        //Seacrh for xlsx password
        std::regex patternBlock1("\\.xlsx(\\s*)\\nE/9(\\s*)\\n.E/9(\\s*)\\n394(\\s*)\\n/x./workbook.xml(\\s*)\\n.+(\\s*)\\nsharedStrings\\.xml");
        std::regex patternBlock2("Microsoft O(\\s*)\\n[A-Z]:(.+)*.*\\.xlsx(\\s*)\\nA~\\{(\\s*)\\n#9(H|')(\\s*)\\n(p3.(\\s*)\\n)*(B~p3.(\\s*)\\n)?B~%(\\s*)\\n(p3.(\\s*)\\n)?D~\\((\\s*)\\n!Z9(H|')(\\s*)\\n.+(\\s*)\\n");
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

            string password = linesOfBlock[5];
            cout<<"Password fond: "<<password<<endl;
        }
        else if(regex_search(text, matches, patternBlock2))
        {
            string block = matches.str();
            
            vector<string> linesOfBlock;
 
            stringstream ss(block);
            string lineOfBlock;
            int counter =0;
            while (getline(ss, lineOfBlock, '\n')) {
                linesOfBlock.push_back(lineOfBlock);
                counter++;
            }
            cout<<"Password fond: "<<linesOfBlock[counter-1]<<endl;
        }
        else
        {
            cout<<"Password not fond."<<endl;
        }
    }
    else if(passwordType == 3)
    {
        //Seacrh for ftp password
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
    //Get path to dump file
    string pathToFile;
    cout<<"Please enter the path to the dump file:"<<endl;
    cin>>pathToFile;

    cout<<"Making text for dump."<<endl;
    string commad = "strings "+pathToFile+" > dumpText.txt";

    //Create text file
    ofstream {"dumpText.txt"};

    //Create file reader
    ifstream MyReadFile("dumpText.txt");
    string line ="";
    string text ="";

    //Execute commad to change dump file to text
    exec(commad.c_str());

    //Get password 
    string password;
    cout<<"Please enter the password:"<<endl;
    cin>>password;

    cout<<"Search for footprint."<<endl;

    string lines[11] = {"","","","","","","","","","",""};

    bool findPassword =false;

    //Search for the password and save the lasted 5 lines and the next 5 lines
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
        //Print out the footprint
        cout<<"Footprint before password:"<<endl;
        for (int counter = 0; counter < 5; counter++)
        {
            cout<<(counter+1)<<' '<<lines[counter]<<endl;
        }
        int startPosition = lines[5].find(password);
        cout<<"Before password: "<<lines[5].substr(0, startPosition)<<endl;

        cout<<"Footprint after password:"<<endl;
        cout<<"After password: "<<lines[5].substr((startPosition + password.length()))<<endl;
        for (int counter = 6; counter < 11; counter++)
        {
            cout<<(counter+1-6)<<' '<<lines[counter]<<endl;
        }
    }
    else
    {
        cout<<"Can not find the footprint."<<endl;
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