#include "session_manager.hpp"
#include "logging.hpp"

#include <iostream>

std::string SessionManager::cookieIdentifier = "PSISESSIONID";
int SessionManager::sessionIdLength = 25;

std::string SessionManager::SessionId(std::string cookies) {
    size_t hasId = cookies.find(cookieIdentifier);

    std::string id = "";
    if (hasId != std::string::npos) {
        id = cookies.substr(hasId + cookieIdentifier.size() + 1, sessionIdLength);
    }
    return id;
}

std::string SessionManager::CookieHeaderValue(std::string & id) {
    return cookieIdentifier + std::string("=") + id;
}


SessionManager* SessionManager::sessionManagerInstance_ = NULL;

SessionManager* SessionManager::Instance() {
    if (!sessionManagerInstance_) {
        sessionManagerInstance_ = new SessionManager();
    }
    return sessionManagerInstance_;
}

SessionManager::SessionManager() : currentSessionId_(""), guidGenerator(sessionIdLength, true) {
}


bool SessionManager::isSession(std::string & id) {
    std::map<std::string, Session>::iterator found = sessions_.find(id);

    if (found != sessions_.end()) {
        return true;
    }
    return false;
}

Session * SessionManager::getSession(std::string & id) {
    std::map<std::string, Session>::iterator found = sessions_.find(id);

    if (found != sessions_.end()) {
        return &(found->second);
    }
    else {
        ERROR("Attempted to get session that not exists!");
    }
    return (new Session(generateNewId()));
}

Session * SessionManager::newSession() {
    std::string id = generateNewId();
    Session * ses = new Session(id);
    addSession(ses);
    return ses;
}

Session * SessionManager::currentSession() {
    return getSession(currentSessionId_);
}

void SessionManager::setCurrentSession(std::string id) {
    currentSessionId_ = id;
}

void SessionManager::addSession(Session * session) {
    sessions_.insert(std::pair<std::string, Session> (session->getId(), *session));
}

std::string SessionManager::generateNewId() {
    return guidGenerator.getGUID();
}
