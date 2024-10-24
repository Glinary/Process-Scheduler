#pragma once
#include "AConsole.h"
class MainConsole : public AConsole
{
public:
	MainConsole();							// Constructor
	~MainConsole() = default;				// Destructor

	// Override the pure virtual functions of AConsole
	void onEnabled() override;
	void display() override;
	void process() override;

	struct Config {							// Struct variable to hold the values of the config.txt
		uint8_t num_cpu;					// Number of CPUs: [1, 128]
		String scheduler;					// or enum SchedulerType { FCFS, RR };
		uint32_t quantum_cycles;			// For Round Robin: [1, 2^32]
		uint32_t batch_process_freq;		// Frequency of batch process: [1, 2^32]
		uint32_t min_ins;					// Minimum instructions: [1, 2^32]
		uint32_t max_ins;					// Maximum instructions: [1, 2^32]
		uint32_t delays_per_exec;			// Delays per execution: [0, 2^32]
	};

	Config getConfig() const;							// Returns the config

private:
	Config config;										// Config variable to hold the values of the config.txt
	void ASCIITextHeader() const;						// Prints the ASCII text header
	void displayDevelopers() const;						// Displays the developers of the program
	bool isValidFirstCommand(String command) const;		// Checks if the first command is valid
	bool validateCommand(String& input) const;			// Validates the command
	bool isValidScreenCommand(String command) const;	// Checks if the command is valid
	void recognizeCommand(String command) const;		// Recognizes the command
	bool loadConfigFile(String& filename);				// Loads the config file
	bool parseConfigFile(String& line);					// Parses the config file
	
};
