#include "../include/ConnectionHandler.h"

using boost::asio::ip::tcp;

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::string;

ConnectionHandler::ConnectionHandler(string host, short port, ThreadSafeHashMap_future &f, SynchronizedHashMap *channelNumber) : host_(host), port_(port), io_service_(),
																																 socket_(io_service_), protocol(f, channelNumber)
{
}

ConnectionHandler::~ConnectionHandler()
{
	close();
}

bool ConnectionHandler::connect()
{
	std::cout << "Starting connect to " << host_ << ":" << port_ << std::endl;
	try
	{
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // Server endpoint
		boost::system::error_code error;
		socket_.connect(endpoint, error);
		if (error)
			throw boost::system::system_error(error);
	}
	catch (std::exception &e)
	{
		std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
		return false;
	}
	return true;
}

bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead)
{
	size_t tmp = 0;
	boost::system::error_code error;
	try
	{
		while (!error && bytesToRead > tmp)
		{
			tmp += socket_.read_some(boost::asio::buffer(bytes + tmp, bytesToRead - tmp), error);
		}
		if (error)
			throw boost::system::system_error(error);
	}
	catch (std::exception &e)
	{
		std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
		return false;
	}
	return true;
}

bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite)
{
	int tmp = 0;
	boost::system::error_code error;
	try
	{
		while (!error && bytesToWrite > tmp)
		{
			tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, bytesToWrite - tmp), error);
		}
		if (error)
			throw boost::system::system_error(error);
	}
	catch (std::exception &e)
	{
		std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
		return false;
	}
	return true;
}

bool ConnectionHandler::getLine(std::string &line)
{
	return getFrameAscii(line, '\0');
}

bool ConnectionHandler::sendLine(std::string &line)
{
	return sendFrameAscii(line, '\0');
}

bool ConnectionHandler::getFrameAscii(std::string &frame, char delimiter)
{
	char ch;

	do
	{
		if (!getBytes(&ch, 1))
		{
			// No data available yet, return false to indicate the caller should retry
			return false;
		}
		if (ch != '\0')
			frame.append(1, ch);
	} while (delimiter != ch);

	return true;
}

bool ConnectionHandler::sendFrameAscii(const std::string &frame, char delimiter)
{
	bool result = sendBytes(frame.c_str(), frame.length());
	if (!result)
		return false;
	return sendBytes(&delimiter, 1);
}

// Close down the connection properly.
void ConnectionHandler::close()
{
	try
	{
		socket_.close();
	}
	catch (...)
	{
		std::cout << "closing failed: connection already closed" << std::endl;
	}
}
Frame ConnectionHandler::process(std::string msg)
{
	return protocol.process(msg);
}
bool ConnectionHandler::shouldTerminate()
{
	return protocol.shouldTerminate();
}
bool ConnectionHandler::hasDataToRead()
{
	boost::asio::socket_base::bytes_readable command(true); // Request bytes available to read
	socket_.io_control(command);							// Query the socket for available data
	return command.get() > 0;								// Returns true if there's data to read
}
bool ConnectionHandler::getLogedIn()
{
	return protocol.getLogedIn();
}