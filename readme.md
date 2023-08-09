# OpenAFE and OpenAFE Desktop App

## Operation

Below is an illustration of how the [OpenAFE](https://github.com/moduhub/openafe) (the original library) works with the OpenAFE Desktop App:
```
+-----------------------------------------+
|				  Desktop				  |
|	====================================  |
|	|        OpenAFE Desktop App       |  |
|	====================================  |
+-----------------------------------------+
				||		/\
			   \  /	   /  \		USB
				\/		||
+-----------------------------------------+
|	====================================  |
|	|           OpenAFE Comm           |  |
|	|      +----------------------+    |  |
|	|      |  OpenAFE Interpreter |    |  |
|	|      +----------------------+    |  |
|	====================================  |
|	|        OpenAFE Executioner       |  |
|	====================================  |
|	|              OpenAFE             |  |
|	====================================  |
|				  Arduino 				  |
+-----------------------------------------+
```

## OpenAFE Comm
The OpenAFE Comm operates as the translation layer between the OpenAFE functions and the OpenAFE Desktop App. And it should be noted that it sits on top of the [OpenAFE library](https://github.com/moduhub/openafe).

### OpenAFE Interpreter
This is an internal module of the OpenAFE Comm, it sole purpose is to get the parameters from the command strings OpenAFE Comm passes to it, returning these parameters inside structs, for better readability and ease use. 


## OpenAFE Executioner
This layer is tasked with calculating the parameters for the AFE IC with the parameters received from OpenAFE Comm, checking if they are not out of bounds, feed the sequencer and start the voltammetry, it is also tasked with sending the data from the AFE to the OpenAFE Comm. 

## OpenAFE Shared
This header file has definitions for constants and structs that are used on OpenAFE Comm, Interpreter and Executioner. But it is not used in the original OpenAFE. 

## OpenAFE Error Codes
This is a header file that contains the definition of multiple errors, and is used by Comm, Interpreter and Executioner.
> It might be added to the OpenAFE root library in the future.

# List of Commands (App to MCU)
## Special Commands
- CVW - Cyclic Voltammetry Wave, sends the parameters and start the voltammetry, if possible.
- DPV - Differential Pulse Voltage, sends the parameters and start the voltammetry, if possible.
- SWV - Square Wave Voltammetry, sends the parameters and start the voltammetry, if possible.
## Standard Commands
> Standard commands have CMD preceding the command code in the command message, for example: `$CMD,RUN*81`.
- RUN - Run (start) the voltammetry process (sequencer only).
- CHK - Check if the AFE is responding properly, reads from the ADIID register.
- DIE - Kills the MCU and AFE, making them 'die' (reset).
- TIA - Configures the Transimpedance Amplifier (TIA) gain resistor.

# List of Messages (MCU to App)
## Special Messages
- SGL - Single point result, has the voltage and current of a single point of the voltammetry.
- MLT - Multi point result, has the voltage and current of multiple points of the voltammetry.
## Standard Messages
> Standard messages have a MSG preceding the message code, for example: `$MSG,RDY*3A`.
- RDY - Device is ready to receive commands from the App.
## Error Messages
> Error messages have a ERR preceding the error message, for example: `$ERR,INV*38`. 
- GEN - General error, may include an error message.
- INV - Invalid command error, for some reson the command is invalid, CRC check error, etc.
- PAR - Parameter error, one of the parameters passed is out of bounds.
- WAV - Wave error, an error has ocurred during the voltammetry process.
- AFE - AFE error, for any reason the AFE IC does not respond properly, e.g. does not pass the health check.

# FAQ

### Why messages from the App to the MCU are called commands and messages from the MCU to the App are called messages?
- Well, because this is what they are. All the MCU receives from the App are commands, things to do, while the MCU only tells the App about the things going on with the AFE.

### Why some messages/commands are called special?
- It is because they are special in one way or another, in case of special commands (CVW, DPV and SWV) they not only feed with parameters but also start the voltammetry. And in the case of special messages (SGL and MLT), they are the result of voltammetry itself, so they are special for that matter alone.