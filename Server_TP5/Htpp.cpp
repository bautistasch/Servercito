#include <iostream>

#include <fstream>
#include <string> 
#include "Http.h"

#define CRLF "\r\n"
#define HOST "127.0.0.1"
  
enum states{S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, ERROR, ERROR_404} ;

Http::Http()
{
    inputTCP = "";
    HtppResponse = "";;
    HtppBodyResponse = "";;
    HtppHeaderResponse = "";;

    HttpPath = "";;
}


std::string Http::getAnswer(std::string dataRead)
{
    inputTCP = dataRead;
    HtppIntrepratateInput();
    return HtppResponse;
}

void Http::HtppIntrepratateInput()
{
    enum states state = S1;

    int txtIndex = 0;
    
    //Recorre todods los estados mientras no haya errores.
    while (state != ERROR && state != S10 && state != ERROR_404)
    {

        //Estado inicial.
        if (state == S1)
        {
            std::string myWord = getWord(&txtIndex);
            if (myWord == "GET")
            {
                state = S2;
            }
            else
            {
                state = ERROR;
            }
            //Si recibe GET pasa al estado 2.
        }
        //Si está en el estado 2 tiene 2 opciones válidas: ->HTTP/1.1, ->/path/..
        else if (state == S2)
        {
            std::string myWord = getWord(&txtIndex);

            if (myWord == "HTTP/1.1")
            {
                state = S3;
            }
            else 
            {
                std::fstream myFile;
                std::string realPath = myWord.substr(1, myWord.size() - 1 );
                myFile.open(realPath);
                if (myFile.is_open() == false)
                {
                    state = ERROR_404;
                }
                else
                {
                    HttpPath = realPath;
                    state = S4;
                    myFile.close();
                }
            }
        }   
        else if (state == S3)
        {
            std::string myWord = getWord(&txtIndex);
            std::fstream myFile;
            std::string realPath = myWord.substr(1, myWord.size() - 1);
            myFile.open(realPath);
            if (myFile.is_open() == false)
            {
                state = ERROR_404;
            }
            else
            {
                HttpPath = realPath;
                state = S5;
                myFile.close();
            }
        }
        else if (state == S4)
        {
            std::string myWord = getWord(&txtIndex);
            if (myWord == "HTTP/1.1")
            {
                state = S5;
            }
            else
            {
                state = ERROR;
            }
        }
        else if (state == S5)
        {
            std::string myWord = getWord(&txtIndex);
            if (myWord == CRLF)
            {
                state = S6;
            }
            else
            {
                state = ERROR;
            }
        }
        else if (state == S6)
        {
            std::string myWord = getWord(&txtIndex);
            if (myWord == "Host:")
            {
                state = S7;
            }
            else
            {
                state = ERROR;
            }
        }      
        else if (state == S7)
        {
            std::string myWord = getWord(&txtIndex);
            if (myWord == HOST)
            {
                state = S8;
            }
            else
            {
                state = ERROR;
            }
        }
        else if (state == S8)
        {
            std::string myWord = getWord(&txtIndex);
            if (myWord == CRLF)
            {
                state = S9;
            }
            else 
            {
                state = ERROR;
            }
        }
        //Leer todo hasta CRLF (en este caso no nos importa qué tiene)
        else if (state == S9)
        {
            std::string myWord = getWord(&txtIndex);
            while (myWord != CRLF)
            {
                myWord = getWord(&txtIndex);
            }
            state = S10;
        }
    }
    if (state == S10)
    {
        std::ifstream myReadFile;
        myReadFile.open(HttpPath);
        HtppBodyResponse = "";             // -<<<<<< AL CONSTRUCTOR
        if (myReadFile.is_open()) {
            while (!myReadFile.eof()) {
                HtppBodyResponse += myReadFile.get();
            }
            HtppBodyResponse = HtppBodyResponse.substr(0, HtppBodyResponse.size() - 1);
        }
        myReadFile.close();

        std::string filenameLenght = std::to_string(HtppBodyResponse.length());

        time_t now = time(0);
        tm* nowTm = gmtime(&now);
        char timeNow[256];
        strftime(timeNow, 256, "%a, %d %b %Y %T GMT", nowTm);    //Ej: Tue, 04 Sep 2018 18:21:49 GMT

        time_t exp = now + 30;
        tm* expTm = gmtime(&exp);
        char timeExp[256];
        strftime(timeExp, 256, "%a, %d %b %Y %T GMT", expTm);    //Ej: Tue, 04 Sep 2018 18:21:49 GMT


        HtppHeaderResponse = std::string("HTTP/1.1 200 OK") + CRLF +
            "Date: " +  timeNow  + CRLF +
            "Location: 127.0.0.1" + "/" + HttpPath + CRLF +
            "Cache-Control: max-age=30" + CRLF +
            "Expires: " + timeExp + CRLF +
            "Content-Lenght: " +  filenameLenght + CRLF +
            "Content-Type: text/html; charset=iso-8859-1"; // 
        
        HtppResponse = HtppHeaderResponse + CRLF + CRLF + HtppBodyResponse;
    }
    else if (state == ERROR_404 || state == ERROR)
    {
        time_t now = time(0);
        tm* nowTm = gmtime(&now);
        char timeNow[256];
        strftime(timeNow, 256, "%a, %d %b %Y %T GMT", nowTm);    //Ej: Tue, 04 Sep 2018 18:21:49 GMT

        time_t exp = now + 30;
        tm* expTm = gmtime(&exp);
        char timeExp[256];
        strftime(timeExp, 256, "%a, %d %b %Y %T GMT", expTm);    //Ej: Tue, 04 Sep 2018 18:21:49 GMT

        HtppHeaderResponse = std::string("HTTP/1.1 404 Not Found") + CRLF
            "Date: " + timeNow + CRLF +
            "Cache-Control: public, max-age=30" + CRLF +
            "Expires : " + timeExp + CRLF +
            "Content-Lenght: 0" + CRLF +
            "Content-Type: text/html; charset=iso-8859-1" + CRLF;
        HtppResponse = HtppHeaderResponse;
    }
}

std::string Http::getWord(int * index)
{
    std::string word("");
    bool spaceDetected = false;
    bool crlfDetected = false;

    if (inputTCP[*index] == '\r' && *index < inputTCP.size())
    {
        word += inputTCP[*index] ;
        word += inputTCP[*index + 1];
        *index = *index + 2;
    }
    else
    {
        for (*index; *index < inputTCP.size() && (!spaceDetected || inputTCP[*index] == ' ') && inputTCP[*index] != '\r' ; (*index)++)
        {
            if (inputTCP[*index] == ' ')
            {
                spaceDetected = true;
            }
            else
            {
                word += inputTCP[*index];
            }
        }
    }
    return word;
}
