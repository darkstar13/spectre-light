//author: Davide Pizzolotto
//license: GNU GPLv3

#include "console.hpp"

#ifdef TESTS
int errors_count[5] = {0,0,0,0,0};
#define UNUSED(x) (void)x;
#endif

#undef Console

#ifndef SPECTRE_COLOURED_OUTPUT
#define NOTICE "NOTICE"
#define WARNING "WARNING"
#define ERROR "ERROR"
#define CRITICAL "CRITICAL"
#define SPNRM  ""
#define SPRED  ""
#define SPGRN  ""
#define SPYEL  ""
#define SPBLU  ""
#define SPCYN  ""
#else
#define SPNRM  "\x1B[0m"
#define SPRED  "\x1B[31m"
#define SPGRN  "\x1B[32m"
#define SPYEL  "\x1B[33m"
#define SPBLU  "\x1B[34m"
#define SPCYN  "\x1B[36m"
#define NOTICE SPCYN "NOTICE" SPNRM
#define WARNING SPYEL "WARNING" SPNRM
#define ERROR SPRED "ERROR" SPNRM
#define CRITICAL SPRED "CRITICAL" SPNRM
#endif

Console::Console()
{
    Console::motd();
}

void Console::motd()
{
#ifndef SUPPRESS_MOTD
    fprintf(stdout,"Spectre version %s\nReleased on %s, compiled on %s\n",
            SPECTRE_VERSION,SPECTRE_RELEASE,__DATE__);

#ifdef SPECTRAL
    fprintf(stdout,"Using full-spectrum renderer\n");
#else
    fprintf(stdout,"Using " SPRED "r" SPGRN "g" SPBLU "b" SPNRM " renderer\n");
#endif
#endif
}

void Console::log(const char* m, const char* v)
{
#ifndef TESTS
    if(v == NULL) //TODO or program not launched with verbose flag
        fprintf(stdout,"%s\n",m);
    else
        fprintf(stdout,"%s\n",v);
#else
    UNUSED(m);
    UNUSED(v);
    errors_count[LOG_INDEX]++;
#endif
}

void Console::notice(const char* s)
{
#ifndef TESTS
    fprintf(stdout,"[" NOTICE "] %s\n",s);
#else
    UNUSED(s);
    errors_count[NOTICE_INDEX]++;
#endif
}

void Console::warning(const char* s)
{
#ifndef TESTS
    fprintf(stderr,"[" WARNING "] %s\n",s);
#else
    UNUSED(s);
    errors_count[WARNING_INDEX]++;
#endif
}

void Console::severe(const char* s)
{
#ifndef TESTS
    fprintf(stderr,"[" ERROR "] %s\n",s);
#else
    UNUSED(s);
    errors_count[ERROR_INDEX]++;
#endif
}

void Console::critical(const char* s)
{
#ifndef TESTS
    fprintf(stderr,"[" CRITICAL "] %s\n",s);
    exit(EXIT_FAILURE);
#else
    UNUSED(s);
    errors_count[CRITICAL_INDEX]++;
#endif
}

void Console::log(bool b, const char* m, const char* v)
{
    if(b)
    {
#ifndef TESTS
        if(v == NULL) //TODO or program not launched with verbose flag
            fprintf(stdout,"%s\n",m);
        else
            fprintf(stdout,"%s\n",v);
#else
        UNUSED(m);
        UNUSED(v);
        errors_count[LOG_INDEX]++;
#endif
    }
}

void Console::notice(bool b, const char* s)
{
    if(b)
    {
#ifndef TESTS
        fprintf(stdout,"[" NOTICE "] %s\n",s);
#else
        UNUSED(s);
        errors_count[NOTICE_INDEX]++;
#endif
    }
}

void Console::warning(bool b, const char* s)
{
    if(b)
    {
#ifndef TESTS
        fprintf(stderr,"[" WARNING "] %s\n",s);
#else
        UNUSED(s);
        errors_count[WARNING_INDEX]++;
#endif
    }
}

void Console::severe(bool b, const char* s)
{
    if(b)
    {
#ifndef TESTS
        fprintf(stderr,"[" ERROR "] %s\n",s);
#else
        UNUSED(s);
        errors_count[ERROR_INDEX]++;
#endif
    }
}

void Console::critical(bool b, const char* s)
{
    if(b)
    {
#ifndef TESTS
        fprintf(stderr,"[" CRITICAL "] %s\n",s);
        exit(EXIT_FAILURE);
#else
        UNUSED(s);
        errors_count[CRITICAL_INDEX]++;
#endif
    }
}

void Console::progress_bar_done()const
{
#ifdef WIN32
    std::cout<<"\r(####################) "<<SPGRN<< "100% Done!"<<SPNRM
              <<std::endl;
#else
    std::cout<<"\33[2K\r(####################) "<<SPGRN<< "100% Done!"<<SPNRM
             <<std::endl;
#endif
}

void Console::progress_bar(float done, float eta)const
{
    done*=20;
    const char fullblock = '#';
    const char emptyblock = '-';
    std::string progress;

    int i=0;
    while(i<(int)done)
    {
        progress.push_back(fullblock);
        i++;
    }
    while(i<20)
    {
        progress.push_back(emptyblock);
        i++;
    }
#ifdef WIN32
    std::cout<<"("<<progress<<") "<<(int)(done*5)<<"%\tETA:"<<eta<<" s\r";
#else
    std::cout<<"\33[2K\r("<<progress<<") "<<(int)(done*5)<<"%\tETA:"<<eta<<" s";
#endif
    fflush(stdout);
}
