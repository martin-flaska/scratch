#include <iostream>
#include <map>
#include <string>
#include <functional>

//
// supporting tools and software
//
// Validate and test your json commands
// https://jsonlint.com/

// RapidJSON : lots and lots of examples to help you use it properly
// https://github.com/Tencent/rapidjson
//

// std::function
// std::bind
// std::placeholders
// std::map
// std::make_pair

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace std;

bool g_done = false;

//
// TEST COMMANDS
//
auto help_command = R"(
 {
  "command":"help",
  "payload": {
    "usage":"Enter json command in 'command':'<command>','payload': { // json payload of arguments }",
  }
 }
)";

auto exit_command = R"(
 {
  "command":"exit",
  "payload": {
     "reason":"Exiting program on user request."
  }
 }
)";

auto hello_command = R"(
 {
  "command":"hello",
  "payload": {
     "message":"Hello world!"
  }
 }
)";

auto calculate_command = R"(
 {
  "command":"calculate",
  "payload": {
     "formula":"1+1"
  }
 }
)";

auto error_command = R"(
 {
  "command":"error`",
  "payload": {
     "message":"Oops..."
  }
 }
)";

class Controller {
public:
    bool help(const rapidjson::Value &payload)
    {
        cout << "Controller::help: command: ";

		if (!payload.IsObject()
			|| !payload["usage"].IsString()
		) {
			cerr << "Missing 'usage' value in payload." << endl;
			return false;
		}

		string usage(payload["usage"].GetString());

		cout << "help: usage: '" << usage << "'" << endl;

        return true;
    }

    bool exit(const rapidjson::Value &payload)
    {
        cout << "Controller::exit: command: \n";

        if (!payload.IsObject()
			|| !payload["reason"].IsString()
		) {
			cerr << "Missing 'reason' value in payload." << endl;
			return false;
		}

		string reason(payload["reason"].GetString());

		cout << "exit: reason: '" << reason << "'" << endl;

        return true;
    }

    bool hello(const rapidjson::Value &payload)
    {
        cout << "Controller::hello: command: \n";

        if (!payload.IsObject()
			|| !payload["message"].IsString()
		) {
			cerr << "Missing 'message' value in payload." << endl;
			return false;
		}

		string message(payload["message"].GetString());

		cout << "hello: message: '" << message << "'" << endl;

        return true;
    }

	bool calculate(const rapidjson::Value &payload)
    {
        cout << "Controller::calculate: command: \n";

        if (!payload.IsObject()
			|| !payload["formula"].IsString()
		) {
			cerr << "Missing 'formula' value in payload." << endl;
			return false;
		}

		string formula(payload["formula"].GetString());

		cout << "calculate: formula: '" << formula << "'" << endl;

        return true;
    }

    bool error(const rapidjson::Value &payload)
    {
        cout << "Controller::error: command: \n";

        if (!payload.IsObject()
			|| !payload["message"].IsString()
		) {
			cerr << "Missing 'message' value in payload." << endl;
			return false;
		}

		string message(payload["message"].GetString());

		cout << "error: message: '" << message << "'" << endl;

        return true;
    }

    // implement 3-4 more commands
};

// gimme ... this is actually tricky
// Bonus Question: why did I type cast this?
// A: to not copy&paste function prototype?
typedef std::function<bool(const rapidjson::Value &)> CommandHandler;

class CommandDispatcher {
public:
    // ctor - need impl
    CommandDispatcher()
    {
    }

    // dtor - need impl
    virtual ~CommandDispatcher()
    {
        // question why is it virtual? Is it needed in this case?
		// A: not neeed in this case (unless another dispatcher class is derived from this)
    }

    bool addCommandHandler(std::string command, CommandHandler handler)
    {
        cout << "CommandDispatcher: addCommandHandler: " << command << std::endl;

		if (command.size() == 0) {
			return false;
		}

        const auto [it, success] = command_handlers_.insert({command, handler});

        return success;
    }

    bool dispatchCommand(std::string command_json)
    {
        cout << "COMMAND: " << command_json << endl;

		Document document;
		// TODO: handle parsing errors
		document.Parse(command_json.data());

		if (!document.HasMember("command")
			|| !document["command"].IsString()
		) {
			cerr << "Missing command value." << endl;
			return false;
		}

		string command = document["command"].GetString();

		auto search = command_handlers_.find(command);
		if (search == command_handlers_.end()) {
			cerr << "Non-existing command: " << command << endl;
			return false;
		}

		if (!document.HasMember("payload")
			|| !document["payload"].IsObject()
		) {
			cerr << "Missing payload object." << endl;
			return false;
		}

		const Value &payload = document["payload"];

		return search->second(payload);
    }

private:

    // gimme ...
    std::map<std::string, CommandHandler> command_handlers_;

    // another gimme ...
    // Question: why delete these?
	// A: to make this object/class not copyable

    // delete unused constructors
    CommandDispatcher (const CommandDispatcher&) = delete;
    CommandDispatcher& operator= (const CommandDispatcher&) = delete;

};

int main()
{
    std::cout << "COMMAND DISPATCHER: STARTED" << std::endl;

    CommandDispatcher command_dispatcher;
    Controller controller;                 // controller class of functions to "dispatch" from Command Dispatcher

    // Implement
    // add command handlers in Controller class to CommandDispatcher using addCommandHandler
    // Note: Controller's methods could be maybe static to ease this
    command_dispatcher.addCommandHandler("help", [&controller](const rapidjson::Value &v) { return controller.help(v); } );
	command_dispatcher.addCommandHandler("exit", [&controller](const rapidjson::Value &v) { return controller.exit(v); } );
	command_dispatcher.addCommandHandler("hello", [&controller](const rapidjson::Value &v) { return controller.hello(v); } );
	command_dispatcher.addCommandHandler("calculate", [&controller](const rapidjson::Value &v) { return controller.calculate(v); } );
	command_dispatcher.addCommandHandler("error", [&controller](const rapidjson::Value &v) { return controller.error(v); } );

    // gimme ...
    // command line interface
    string command;
    while( ! g_done ) {
        cout << "COMMANDS: {\"command\":\"exit\", \"payload\":{\"reason\":\"User requested exit.\"}}\n";
        cout << "\tenter command : (available: help, exit, hello, calculate, error)";
        getline(cin, command);
        command_dispatcher.dispatchCommand(command);
    }

    std::cout << "COMMAND DISPATCHER: ENDED" << std::endl;
    return 0;
}
