#ifndef GREEN_CREATETRANSACTIONHANDLER_H
#define GREEN_CREATETRANSACTIONHANDLER_H

#include "handler.h"

class CreateTransactionHandler : public Handler
{
    const QJsonObject m_details;
    void call(GA_session* session, GA_auth_handler** auth_handler) override;
public:
    CreateTransactionHandler(Wallet* wallet, const QJsonObject& details);
};

#endif // GREEN_CREATETRANSACTIONHANDLER_H
