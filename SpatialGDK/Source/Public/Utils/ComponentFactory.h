// Copyright (c) Improbable Worlds Ltd, All Rights Reserved

#pragma once

#include "Interop/SpatialTypebindingManager.h"
#include "Utils/RepDataUtils.h"

#include <WorkerSDK/improbable/c_schema.h>
#include <WorkerSDK/improbable/c_worker.h>

class USpatialNetDriver;
class USpatialPackageMap;
class USpatialTypebindingManager;
class USpatialPackageMapClient;

class UNetDriver;
class UProperty;

enum EReplicatedPropertyGroup : uint32;

using FUnresolvedObjectsMap = TMap<Schema_FieldId, TSet<const UObject*>>;
using FResolvedActorProxyMap = TMap<Schema_FieldId, TSet<const UObject*>>;

namespace improbable
{

class SPATIALGDK_API ComponentFactory
{
public:
	ComponentFactory(FUnresolvedObjectsMap& RepUnresolvedObjectsMap, FUnresolvedObjectsMap& HandoverUnresolvedObjectsMap, USpatialNetDriver* InNetDriver);

	TArray<Worker_ComponentData> CreateComponentDatas(UObject* Object, FClassInfo* Info, const FRepChangeState& RepChangeState, const FHandoverChangeState& HandoverChangeState);
	TArray<Worker_ComponentUpdate> CreateComponentUpdates(UObject* Object, FClassInfo* Info, Worker_EntityId EntityId, const FRepChangeState* RepChangeState, const FHandoverChangeState* HandoverChangeState);

	static Worker_ComponentData CreateEmptyComponentData(Worker_ComponentId ComponentId);

private:
	Worker_ComponentData CreateComponentData(Worker_ComponentId ComponentId, UObject* Object, const FRepChangeState& Changes, ESchemaComponentType PropertyGroup);
	Worker_ComponentUpdate CreateComponentUpdate(Worker_ComponentId ComponentId, UObject* Object, const FRepChangeState& Changes, ESchemaComponentType PropertyGroup, bool& bWroteSomething);

	bool FillSchemaObject(Schema_Object* ComponentObject, UObject* Object, const FRepChangeState& Changes, ESchemaComponentType PropertyGroup, bool bIsInitialData, TArray<Schema_FieldId>* ClearedIds = nullptr);

	Worker_ComponentData CreateHandoverComponentData(Worker_ComponentId ComponentId, UObject* Object, FClassInfo* Info, const FHandoverChangeState& Changes);
	Worker_ComponentUpdate CreateHandoverComponentUpdate(Worker_ComponentId ComponentId, UObject* Object, FClassInfo* Info, const FHandoverChangeState& Changes, bool& bWroteSomething);

	bool FillHandoverSchemaObject(Schema_Object* ComponentObject, UObject* Object, FClassInfo* Info, const FHandoverChangeState& Changes, bool bIsInitialData, TArray<Schema_FieldId>* ClearedIds = nullptr);

	Worker_ComponentData CreateInterestComponentData(UObject* Object, bool& bWroteSomething, Worker_EntityId EntityId);
	Worker_ComponentUpdate CreateInterestComponentUpdate(UObject* Object, bool& bWroteSomething, Worker_EntityId EntityId);

	bool FillInterestSchemaObject(Schema_Object* ComponentObject, Worker_EntityId EntityId);

	USpatialNetDriver* NetDriver;
	USpatialPackageMapClient* PackageMap;
	USpatialTypebindingManager* TypebindingManager;

	FUnresolvedObjectsMap& PendingRepUnresolvedObjectsMap;
	FUnresolvedObjectsMap& PendingHandoverUnresolvedObjectsMap;

	FResolvedActorProxyMap ResolvedChangedActorProxyMap;
};

}
