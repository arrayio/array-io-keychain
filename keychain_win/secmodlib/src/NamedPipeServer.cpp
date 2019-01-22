#include "NamedPipeServer.h"

#include <keychain_lib/pipeline_parser.hpp>

#include <fcntl.h>
#include <io.h>

#include <iostream>
#include <future>

#include "SecureModuleWrapper.h"
#include <keychain_lib/keychain_logger.hpp>

NamedPipeServer::NamedPipeServer() {
  //_secManage = new SecurityManager();
}

NamedPipeServer::~NamedPipeServer() {

}

using namespace keychain_app;

void NamedPipeServer::ListenChannel(/*LPTSTR channelName*/) {

  lpszPipename = (LPTSTR)__TEXT("\\\\.\\pipe\\keychainservice");//channelName;
  auto log = logger_singleton::instance();
  BOOST_LOG_SEV(log.lg, info) << "Creating NamedPipe object";

  hPipe = CreateNamedPipe(
    lpszPipename,             // pipe name 
    PIPE_ACCESS_DUPLEX,       // read/write access 
    PIPE_TYPE_MESSAGE |       // message type pipe 
    PIPE_READMODE_MESSAGE |   // message-read mode 
    PIPE_WAIT,                // blocking mode 
    PIPE_UNLIMITED_INSTANCES, // max. instances  
    BUFSIZE,                  // output buffer size 
    BUFSIZE,                  // input buffer size 
    0,                        // client time-out 
    NULL);                    // default security attribute 

  BOOST_LOG_SEV(log.lg, info) << "NamedPipe object is created";

  fConnected = ConnectNamedPipe(hPipe, NULL) ?
    TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
  BOOST_LOG_SEV(log.lg, info) << "ConnectingNamePipe";
  if (!fConnected)
  {
    BOOST_LOG_SEV(log.lg, error) << "Connection error";
    BOOST_LOG_SEV(log.lg, error) << std::to_string(GetLastError());
    // The client could not connect, so close the pipe. 
    CloseHandle(hPipe);
    return;
  }

  auto fd = _open_osfhandle(reinterpret_cast<intptr_t>(hPipe), _O_APPEND | _O_RDWR);

  BOOST_LOG_SEV(log.lg, info) << "Client connected, creating a processing thread.";
  auto res = std::async(std::launch::async, [this](int fd)->int {
    SecureModuleWrapper secureModuleWrapper;
    auto& keychain_ref = keychain::instance();
    secureModuleWrapper.connect(keychain_ref);
    keychain_invoke_f f = std::bind(&keychain_base::operator(), &keychain_ref, std::placeholders::_1);
    pipeline_parser pipe_line_parser_(std::move(f), fd, fd);
    int res = pipe_line_parser_.run();
    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    return res;
  }, fd);
  try
  {
    if (res.get() == -1)
      BOOST_LOG_SEV(log.lg, error) << "Error: cannot read from pipe";
  }
  catch (const std::exception& e)
  {
    BOOST_LOG_SEV(log.lg, error) << "Error: cannot read from pipe";
    BOOST_LOG_SEV(log.lg, error) << e.what();
  }
};
