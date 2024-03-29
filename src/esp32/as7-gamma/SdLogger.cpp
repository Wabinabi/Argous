#include "SdLogger.h"

namespace AS7
{
    

    void Logger::mainTask(void * parameters) { 
        std::string msg;
        for (;;) {
            xSemaphoreTake(getSemEnableMutex(), portMAX_DELAY);
            

            // Check the log size
            xSemaphoreTake(getSemLogQueueMutex(), portMAX_DELAY);
            openLogFile();
            while (!getLogQueue().empty()) {
              msg = dequeueLog();
              

                getLogFile().println(msg.c_str());
                getPrinter()->println(msg.c_str());
                // write to SD card
                vTaskDelay(1 / portTICK_PERIOD_MS); // limit watchdog starvation
            }
            closeLogFile();
            xSemaphoreGive(getSemLogQueueMutex());
            
            
            xSemaphoreTake(getSemDataEnqMutex(), portMAX_DELAY);
            openDataFile();
            if (_hasEnqueuedData) {

                getDataFile().print((std::to_string(_enqueuedData["Test Version"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["drone_x"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["drone_y"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["drone_z"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["us_0"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["us_1"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["us_2"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["us_3"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["us_4"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["us_5"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["raw_accel_x"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["raw_accel_y"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["raw_accel_z"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["filt_accel_x"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["filt_accel_y"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["filt_accel_z"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["raw_vel_x"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["raw_vel_y"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["raw_vel_z"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["filt_vel_x"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["filt_vel_y"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["filt_vel_z"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["raw_pos_x"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["raw_pos_y"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["raw_pos_z"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["filt_pos_x"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["filt_pos_y"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["filt_pos_z"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["heading"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["compass_x"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["compass_y"])+",").c_str());
                getDataFile().print((std::to_string(_enqueuedData["compass_z"])+",").c_str());
                getDataFile().print(std::to_string(_enqueuedData["millis"]).c_str());
                getDataFile().print(std::to_string(_enqueuedData["recording_enabled"]).c_str());
                getDataFile().print("\n");

                _hasEnqueuedData = false;
            }
            closeDataFile();
            xSemaphoreGive(getSemDataEnqMutex());
            xSemaphoreGive(getSemEnableMutex());
            
            vTaskDelay((1000/LOGGER_FREQ) / portTICK_PERIOD_MS); // Allow other tasks to take control
        }
        
    }

    std::string Logger::dequeueLog() {
      std::string top = _log_Queue.front();
      _log_Queue.pop();
      return top;
    }

    


    void Logger::enqueueMsg(std::string message) {
        //_printer->println(message.c_str());

        //printer->println(message.c_str());
        // log onto SD card

        xSemaphoreGive(_sem_msgQueueMutex);
    }

    void Logger::enqueueLog(std::string message, int verbosity) {
        xSemaphoreTake(_sem_logQueueMutex, portMAX_DELAY);
        _log_Queue.push(message);
        xSemaphoreGive(_sem_logQueueMutex);
    }

    
    Print* Logger::getPrinter() {return _printer;}

    std::queue<std::string> Logger::getMsgQueue() { return _msg_Queue; }
    std::queue<std::string> Logger::getLogQueue() { return _log_Queue; }

    SemaphoreHandle_t Logger::getSemLog() { return _sem_log; }
    SemaphoreHandle_t Logger::getSemMsg() { return _sem_msg; }
    SemaphoreHandle_t Logger::getSemLogQueueMutex() { return _sem_logQueueMutex; }
    SemaphoreHandle_t Logger::getSemMsgQueueMutex() { return _sem_msgQueueMutex; }

    SemaphoreHandle_t Logger::getSemEnableMutex() { return _sem_enableMutex; }

    int Logger::verbosity() {return _verbosity; }
    void Logger::setVerbosity(int verbosity) {_verbosity = verbosity; };

    bool Logger::running() {return _running; }

    void Logger::inform(std::string message) {
        enqueueLog("[ "+std::to_string(millis())+" ms Inform] " + message, LOG_LEVEL_INFORM);
    }

    void Logger::warn(std::string message) {
        enqueueLog("[ "+std::to_string(millis())+" ms Warning] " + message, LOG_LEVEL_WARNING);
    }

    void Logger::error(std::string message) {
        enqueueLog("[ "+std::to_string(millis())+" ms Error] " + message, LOG_LEVEL_ERROR);
    }

    void Logger::fatal(std::string message) {
        enqueueLog("[ "+std::to_string(millis())+" ms Fatal] " + message, LOG_LEVEL_FATAL);
    }

    void Logger::verbose(std::string message) {
        enqueueLog("[ "+std::to_string(millis())+" ms Verbose] " + message, LOG_LEVEL_VERBOSE);
    }

    void Logger::plot(std::string message) {
        enqueueLog(message, LOG_LEVEL_VERBOSE);
    }

    void Logger::startTaskImpl(void* _this) {
    ((Logger*)_this)->mainTask(NULL);
    }

    void Logger::recordData(std::string key, float value) {
        xSemaphoreTake(_sem_dataMutex, portMAX_DELAY);
        _activeData[key] = value;
        xSemaphoreGive(_sem_dataMutex);
        
    }

    void Logger::pushData() {
        xSemaphoreTake(_sem_dataEnqMutex, portMAX_DELAY);
        _activeData["millis"] = millis();
        _enqueuedData = _activeData;
        _hasEnqueuedData = true;
        xSemaphoreGive(_sem_dataEnqMutex);
    }

    void Logger::initialiseSD() {
        if (!SD_DISABLED) {
            if (!SD.begin(CS_PIN)) {
            fatal("SD initialisation failed, is the SD module loose or not connected?");
            verbose("SD.Begin(CS_PIN) failed to return a true value.");
            } else {
                _sdDetected = true;
                inform("SD card detected. SD Logging enabled.");
                inform("(C) 2022 Ecobat Project");

                // Clear previous logs
                File logFile = SD.open(_logFileLocation.c_str(), FILE_WRITE);

                if (logFile) {
                    logFile.println("AS7 Log file - (C) Ecobat Project 2022");
                    logFile.close();
                } else {
                    fatal("AS7 Log file could not be written to!");
                }

                // Clear and set CSV
                logFile = SD.open(_dataFileLocation.c_str(), FILE_WRITE);

                if (logFile) {
                    logFile.print("Test Version, drone_x, drone_y, drone_z, us_0, us_1, us_2, us_3, us_4, us_5, ");
                    logFile.print("raw_accel_x, raw_accel_y, raw_accel_z, filt_accel_x, filt_accel_y, filt_accel_z, ");
                    logFile.print("raw_vel_x, raw_vel_y, raw_vel_z, filt_vel_x, filt_vel_y, filt_vel_z, ");
                    logFile.print("raw_pos_x, raw_pos_y, raw_pos_z, filt_pos_x, filt_pos_y, filt_pos_z, ");
                    logFile.print("heading, compass_x, compass_y, compass_z, millis, recording_enabled");
                    logFile.println();
                    _activeData["Test Version"]=0;
                    _activeData["drone_x"]=0;
                    _activeData["drone_y"]=0;
                    _activeData["drone_y"]=0;
                    _activeData["us_0"]=0;
                    _activeData["us_1"]=0;
                    _activeData["us_2"]=0;
                    _activeData["us_3"]=0;
                    _activeData["us_4"]=0;
                    _activeData["us_5"]=0;
                    _activeData["raw_accel_x"]=0;
                    _activeData["raw_accel_y"]=0;
                    _activeData["raw_accel_z"]=0;
                    _activeData["filt_accel_x"]=0;
                    _activeData["filt_accel_y"]=0;
                    _activeData["filt_accel_z"]=0;
                    _activeData["raw_vel_x"]=0;
                    _activeData["raw_vel_y"]=0;
                    _activeData["raw_vel_z"]=0;
                    _activeData["filt_vel_x"]=0;
                    _activeData["filt_vel_y"]=0;
                    _activeData["filt_vel_z"]=0;
                    _activeData["raw_pos_x"]=0;
                    _activeData["raw_pos_y"]=0;
                    _activeData["raw_pos_z"]=0;
                    _activeData["filt_pos_x"]=0;
                    _activeData["filt_pos_y"]=0;
                    _activeData["filt_pos_z"]=0;
                    _activeData["heading"]=0;
                    _activeData["compass_x"]=0;
                    _activeData["compass_y"]=0;
                    _activeData["compass_z"]=0;
                    _activeData["millis"]=0;
                    _activeData["recording_enabled"]=0;
                    logFile.close();
                } else {
                    fatal("Data CSV could not be written to!");
                }

            }
            
        } else {
            warn("SD Logging has been disabled globally! Data will not be recorded to SD");
        }
        
        
    }


    // We're going with the assumption that there's only one logger
    //  For now we won't be implementing protection here since it could cause debugging issues
    void Logger::pause() {
        if (!_running) {
            warn("Request to pause logger denied: Logger already paused.");
        } else {
            _running = false;
            xSemaphoreTake(getSemEnableMutex(), portMAX_DELAY);
        }
    }

    void Logger::resume() {
        if (!_running) {
            _running = true;
            xSemaphoreGive(_sem_enableMutex);
        } else {
            warn("Request to resume logger denied: Logger already running");
        }
        
    }

    void Logger::start(int core, int priority, int verbosity) {
        _verbosity = verbosity;
        
        xTaskCreatePinnedToCore(
        this->Logger::startTaskImpl,                /* Task function. */
        "Logger",              /* name of task. */
        8192,                   /* Queue size of task */
        this,                   /* parameter of the task */
        priority, /* priority of the task */
        &th_logger,         /* Task handle to keep track of created task */
        core);                     /* pin task to core 1 */

        _running = true;
        xSemaphoreGive(_sem_enableMutex);
    }

    File Logger::getLogFile() {
        return _logFile;
    }
    File Logger::getConfigFile() {
        return _configFile;
    }
    File Logger::getDataFile() {
        return _dataFile;
    }

    void Logger::openLogFile() {
        _logFile = SD.open(_logFileLocation.c_str(), FILE_APPEND);
    }

    void Logger::closeLogFile() {
        _logFile.close();
    }

    void Logger::openDataFile() {
        _dataFile = SD.open(_dataFileLocation.c_str(), FILE_APPEND);
    }

    void Logger::closeDataFile() {
        _dataFile.close();
    }

    void Logger::disableSDLogging() {
        _sdEnabled = true;
        warn("SD logging has been disabled! Results will not be recorded to SD Card");
    }
    
    Logger::Logger(Print* output) {
        _printer = output;

        _sem_logQueueMutex = xSemaphoreCreateBinary();
        _sem_enableMutex = xSemaphoreCreateBinary();
        _sem_dataMutex = xSemaphoreCreateBinary();
        _sem_dataEnqMutex = xSemaphoreCreateBinary();
        
        xSemaphoreGive(_sem_logQueueMutex);
        xSemaphoreGive(_sem_dataMutex);
        xSemaphoreGive(_sem_dataEnqMutex);

        initialiseSD();
        
    }
}
