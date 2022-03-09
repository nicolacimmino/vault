#ifndef __VAULT_RETRIEVE_PASSWORD_SERVICE_H__
#define __VAULT_RETRIEVE_PASSWORD_SERVICE_H__

#include "Service.h"
#include "Terminal.h"
#include <Keyboard.h>
#include "hardware.h"
#include <EEPROM.h>
#include "EncryptedStore.h"
#ifndef HW_BUTTON
#include <ADCTouch.h>
#endif
#include "NotificationController.h"

#define BACKUP_LINE_SIZE 64
#define RETRIEVE_PASSWORD_ACTION_FULL 0
#define RETRIEVE_PASSWORD_ACTION_PARTIAL 1
#define RETRIEVE_PASSWORD_ACTION_SHOW 2
#define PASSWORD_TYPE_INTERDIGIT_DELAY_PARTIAL 300

class RetrievePasswordService : public Service
{
private:
    Terminal *terminal;
    EncryptedStore *encryptedStore;
    NotificationController *notificationController;
    byte selectedPasswordIndex;
    void typeClipboard(char *clipboard, uint16_t interDigitDelay);

public:
    RetrievePasswordService(Terminal *terminal, EncryptedStore *encryptedStore, NotificationController *notificationController, byte selectedPasswordIndex, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion);
    bool start(byte arg = 0);
    bool loop();
};

#endif