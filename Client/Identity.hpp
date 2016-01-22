#ifndef INDENTITY_HPP_
# define INDENTITY_HPP_

# include <iostream>
# include <vector>
# include <cstring>

enum Instruct
{
    CONNECTION,
    DECONNECTION,
    ADDCONTACT,
    ASKCONTACT,
    DELCONTACT,
    ASKCALL,
    RCVCALL,
    CALLESTABLISHED,
    CALLFAILED,
    ENDCALL,
    OK,
    KO
};

# define IDCLASSSIZE 104

class Identity
{
public:

    Identity(std::string const& name, Instruct const instruction)
    {
        memset(_username, 0, sizeof(_username));
        memset(_ip, 0, sizeof(_ip));
        for (unsigned int i = 0; i < name.length(); ++i)
            _username[i] = name[i];
        _port = 0;
        _instruction = instruction;
    }

    Identity(std::string const& name, std::string const& ip, unsigned int const port, Instruct const instruction)
    {
        memset(_username, 0, sizeof(_username));
        memset(_ip, 0, sizeof(_ip));
        for (unsigned int i = 0; i < name.length(); ++i)
            _username[i] = name[i];
        for (unsigned int i = 0; i < ip.length(); ++i)
            _ip[i] = ip[i];
        _port = port;
        _instruction = instruction;
    }

    Instruct    getInstruct() const
    {
        return (_instruction);
    }

    void        setInstruct(Instruct instruct)
    {
        this->_instruction = instruct;
    }

    char    *getUsername()
    {
        return (_username);
    }

    char    *getIp()
    {
        return (_ip);
    }

    int     getPort() const
    {
        return (_port);
    }

    bool    hasAdressAndName() const
    {
        return (strlen(this->_ip) != 0 && this->_port != 0 && strlen(this->_username) != 0);
    }

    bool    hasName() const
    {
        return (strlen(this->_username) != 0);
    }

    std::vector<unsigned char>&     serialize()
    {
        std::vector<unsigned char>  *serialized = new std::vector<unsigned char>(IDCLASSSIZE);
        unsigned char *instr;
        unsigned char *port;

        instr = reinterpret_cast<unsigned char *>(&_instruction);
        port = reinterpret_cast<unsigned char *>(&_port);

        for (unsigned int i = 0; i < 4; ++i)
        {
            (*serialized)[i] = instr[i];
            (*serialized)[i + sizeof(Instruct)] = port[i];
        }
        for (unsigned int i = 0; i < sizeof(_username); ++i)
        {
            (*serialized)[sizeof(Instruct) + sizeof(unsigned int) + i] = _username[i];
            if (i < 32)
                (*serialized)[sizeof(Instruct) + sizeof(unsigned int) + sizeof(_username) + i] = _ip[i];
        }
        return (*serialized);
    }

    static Identity&    unserialize(std::vector<unsigned char>& data)
    {
        Instruct             *_instruction;
        unsigned int         *_port;
        char        _username[64];
        char        _ip[32];

        _instruction = reinterpret_cast<Instruct *>(&data[0]);
        _port = reinterpret_cast<unsigned int *>(&data[4]);
        for (unsigned int i = 0; i < sizeof(_username); ++i)
        {
            _username[i] = data[i + sizeof(Instruct) + sizeof(unsigned int)];
            if (i < 32)
                _ip[i] = data[i + sizeof(_username) + sizeof(Instruct) + sizeof(unsigned int)];
        }
        Identity *id = new Identity(_username, _ip, *_port, *_instruction);
        return (*id);
    }

private:
    Instruct        _instruction;
    unsigned int    _port;
    char            _username[64];
    char            _ip[32];
};

#endif
