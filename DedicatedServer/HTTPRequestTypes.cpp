#include "HTTPRequestTypes.h"
#include "Common/DedicatedServer.h"

void FDSMetaData::Dump() const
{
	UE_LOG(LogAuraDS, Log, TEXT("----- Meta Data -----"));
	UE_LOG(LogAuraDS, Log, TEXT("Http Status Code: %d"), httpStatusCode);
	UE_LOG(LogAuraDS, Log, TEXT("Request ID: %s"), *requestId);
	UE_LOG(LogAuraDS, Log, TEXT("Attempts: %d"), attempts);
	UE_LOG(LogAuraDS, Log, TEXT("Total Retry Delay: %f"), totalRetryDelay);
}

void FDSListFleetsResponse::Dump() const
{
	UE_LOG(LogAuraDS, Log, TEXT("----- FDSListFleetsResponse -----"));
	for (const FString& FleetId : FleetIds)
	{
		UE_LOG(LogAuraDS, Log, TEXT("Fleet ID: %s"), *FleetId);
	}
	if (!NextToken.IsEmpty())
	{
		UE_LOG(LogAuraDS, Log, TEXT("Next Token: %s"), *NextToken);
	}
}

void FDSCodeDeliveryDetails::Dump() const
{
	UE_LOG(LogAuraDS, Log, TEXT("----- FDSCodeDeliveryDetails -----"));
	UE_LOG(LogAuraDS, Log, TEXT("AttributeName: %s"), *AttributeName);
	UE_LOG(LogAuraDS, Log, TEXT("DeliveryMedium: %s"), *DeliveryMedium);
	UE_LOG(LogAuraDS, Log, TEXT("Destination: %s"), *Destination);
}

void FDSSignUpResponse::Dump() const
{
	UE_LOG(LogAuraDS, Log, TEXT("----- FDSConfirmSignUpResponse -----"));
	UE_LOG(LogAuraDS, Log, TEXT("Session: %s"), *Session);
	UE_LOG(LogAuraDS, Log, TEXT("UserConfirmed: %s"), UserConfirmed ? TEXT("true") : TEXT("false"));
	UE_LOG(LogAuraDS, Log, TEXT("UserSub: %s"), *UserSub);
	CodeDeliveryDetails.Dump();
}

void FDSNewDeviceMetadata::Dump() const
{
	UE_LOG(LogAuraDS, Log, TEXT("----- FDSNewDeviceMetadata -----"));
	UE_LOG(LogAuraDS, Log, TEXT("DeviceGroupKey: %s"), *DeviceGroupKey);
	UE_LOG(LogAuraDS, Log, TEXT("DeviceKey: %s"), *DeviceKey);
}

void FDSAuthenticationResult::Dump() const
{
	UE_LOG(LogAuraDS, Log, TEXT("----- FDSAuthenticationResult -----"));
	UE_LOG(LogAuraDS, Log, TEXT("AccessToken: %s"), *AccessToken);
	UE_LOG(LogAuraDS, Log, TEXT("ExpiresIn: %d"), ExpiresIn);
	UE_LOG(LogAuraDS, Log, TEXT("IdToken: %s"), *IdToken);
	UE_LOG(LogAuraDS, Log, TEXT("RefreshToken: %s"), *RefreshToken);
	UE_LOG(LogAuraDS, Log, TEXT("TokenType: %s"), *TokenType);
	NewDeviceMetadata.Dump();
}

void FDSChallengeParameters::Dump() const
{
	UE_LOG(LogAuraDS, Log, TEXT("----- FDSChallengeParameters -----"));
	for (const auto& Pair : Parameters)
	{
		UE_LOG(LogAuraDS, Log, TEXT("[%s : %s]"), *Pair.Key, *Pair.Value);
	}
}

void FDSInitiateAuthResponse::Dump() const
{
	UE_LOG(LogAuraDS, Log, TEXT("----- FDSRespondToAuthChallengeResponse -----"));
	
	UE_LOG(LogAuraDS, Log, TEXT("ChallengeName: %s"), *ChallengeName);
	UE_LOG(LogAuraDS, Log, TEXT("Session: %s"), *Session);

	AuthenticationResult.Dump();
	ChallengeParameters.Dump();
}

void FDSGameSession::Dump() const
{
	UE_LOG(LogAuraDS, Log, TEXT("----- FDSGameSession -----"));
	UE_LOG(LogAuraDS, Log, TEXT("CreationTime: %s"), *CreationTime);
	UE_LOG(LogAuraDS, Log, TEXT("CreatorId: %s"), *CreatorId);
	UE_LOG(LogAuraDS, Log, TEXT("CurrentPlayerSessionCount: %d"), CurrentPlayerSessionCount);
	UE_LOG(LogAuraDS, Log, TEXT("DnsName: %s"), *DnsName);
	UE_LOG(LogAuraDS, Log, TEXT("FleetArn: %s"), *FleetArn);
	UE_LOG(LogAuraDS, Log, TEXT("FleetId: %s"), *FleetId);

	UE_LOG(LogAuraDS, Log, TEXT("--- GameProperties:"));
	for (const auto& Pair : GameProperties)
	{
		UE_LOG(LogAuraDS, Log, TEXT("  [%s : %s]"), *Pair.Key, *Pair.Value);
	}

	UE_LOG(LogAuraDS, Log, TEXT("GameSessionData: %s"), *GameSessionData);
	UE_LOG(LogAuraDS, Log, TEXT("GameSessionId: %s"), *GameSessionId);
	UE_LOG(LogAuraDS, Log, TEXT("IpAddress: %s"), *IpAddress);
	UE_LOG(LogAuraDS, Log, TEXT("Location: %s"), *Location);
	UE_LOG(LogAuraDS, Log, TEXT("MatchmakerData: %s"), *MatchmakerData);
	UE_LOG(LogAuraDS, Log, TEXT("MaximumPlayerSessionCount: %d"), MaximumPlayerSessionCount);
	UE_LOG(LogAuraDS, Log, TEXT("Name: %s"), *Name);
	UE_LOG(LogAuraDS, Log, TEXT("PlayerSessionCreationPolicy: %s"), *PlayerSessionCreationPolicy);
	UE_LOG(LogAuraDS, Log, TEXT("Port: %d"), Port);
	UE_LOG(LogAuraDS, Log, TEXT("Status: %s"), *Status);
	UE_LOG(LogAuraDS, Log, TEXT("StatusReason: %s"), *StatusReason);
	UE_LOG(LogAuraDS, Log, TEXT("TerminationTime: %s"), *TerminationTime);
}

void FDSPlayerSession::Dump() const
{
	UE_LOG(LogAuraDS, Log, TEXT("----- FDSPlayerSession -----"));
		   
	UE_LOG(LogAuraDS, Log, TEXT("CreationTime: %s"), *CreationTime);
	UE_LOG(LogAuraDS, Log, TEXT("DnsName: %s"), *DnsName);
	UE_LOG(LogAuraDS, Log, TEXT("FleetArn: %s"), *FleetArn);
	UE_LOG(LogAuraDS, Log, TEXT("FleetId: %s"), *FleetId);
	UE_LOG(LogAuraDS, Log, TEXT("GameSessionId: %s"), *GameSessionId);
	UE_LOG(LogAuraDS, Log, TEXT("IpAddress: %s"), *IpAddress);
	UE_LOG(LogAuraDS, Log, TEXT("PlayerData: %s"), *PlayerData);
	UE_LOG(LogAuraDS, Log, TEXT("PlayerId: %s"), *PlayerId);
	UE_LOG(LogAuraDS, Log, TEXT("PlayerSessionId: %s"), *PlayerSessionId);
	UE_LOG(LogAuraDS, Log, TEXT("Port: %d"), Port);
	UE_LOG(LogAuraDS, Log, TEXT("Status: %s"), *Status);
	UE_LOG(LogAuraDS, Log, TEXT("TerminationTime: %s"), *TerminationTime);
	UE_LOG(LogAuraDS, Log, TEXT("Sub: %s"), *Sub);
}


