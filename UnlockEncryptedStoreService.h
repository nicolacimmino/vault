#ifndef __VAULT_UNLOCK_ENCRYPTED_STORE_SERVICE_H__
#define __VAULT_UNLOCK_ENCRYPTED_STORE_SERVICE_H__

#include "hardware.h"
#include "Service.h"
#include "Terminal.h"
#include "messages.h"
#include "NotificationController.h"

class UnlockEncryptedStoreService : public Service
{
private:
    Terminal *terminal;
    EncryptedStore *encryptedStore;
    NotificationController *notificationController;

public:
    UnlockEncryptedStoreService(Terminal *terminal, EncryptedStore *encryptedStore, NotificationController *notificationController, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion);
    bool start(byte arg = 0);
    bool loop();
};

#endif