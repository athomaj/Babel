#include "Server.hh"

int     main()
{
  try
  {
    Server        serv(4242);
    serv.select_manage();
  }
  catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return (0);
}
