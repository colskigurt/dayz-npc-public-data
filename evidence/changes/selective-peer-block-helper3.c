// Keep opaque dBlock values in typed fields, never a generic dBlock array.
class NFSelectivePeerBlockEntry
{
 dBlock Handle;Animal_DGNativeProbe Owner;
}
// Experimental mission helper. Default OFF; never changes global layers or LOD.
class NFSelectivePeerBlockSet
{
 ref array<ref NFSelectivePeerBlockEntry> Entries={};
 int CreatedTotal;int RemoveRequests;int Expected;int AuditLeaderPairsSkipped;
 string Failure;
 bool Release()
 {
  foreach(NFSelectivePeerBlockEntry checkedEntry:Entries)
   if(!checkedEntry || !checkedEntry.Owner || !checkedEntry.Handle){Failure="OWNER_OR_HANDLE_MISSING";return false;}
  for(int i=Entries.Count() - 1;i>=0;i--)
  {
   NFSelectivePeerBlockEntry entry=Entries[i];
   Animal_DGNativeProbe blockOwner=entry.Owner;dBlock localBlock=entry.Handle;
   dBodyRemoveBlock(blockOwner,localBlock);RemoveRequests++;
  }
  Entries.Clear();return true;
 }
 bool Install(array<ref array<Animal_DGNativeProbe>> groups,array<Animal_DGNativeProbe> leaders,array<Animal_DGNativeProbe> receivers,bool enabled=false,int forceFailureAfter=-1)
 {
  Failure="";
  if(Entries.Count()!=0){Failure="ALREADY_INSTALLED";return false;}
  Expected=0;AuditLeaderPairsSkipped=0;
  if(!enabled)return true;
  int count=groups.Count();
  if(count<2 || count>64 || leaders.Count()!=count || (receivers.Count()!=0 && receivers.Count()!=count)){Failure="GROUP_COUNT";return false;}
  ref array<Animal_DGNativeProbe> seen={};ref array<Animal_DGNativeProbe> seenReceivers={};
  for(int g=0;g<count;g++)
  {
   if(!groups[g] || groups[g].Count()!=5 || !leaders[g]){Failure="ROLE_COUNT";return false;}
   if(groups[g][0]!=leaders[g]){Failure="LEADER_IDENTITY";return false;}
   if(receivers.Count()>0)
   {
    if(!receivers[g] || !dBodyIsSet(receivers[g]) || seenReceivers.Find(receivers[g])>=0){Failure="RECEIVER_IDENTITY_OR_PHYSICS";return false;}
    seenReceivers.Insert(receivers[g]);
   }
   for(int p=0;p<5;p++)
   {
    Animal_DGNativeProbe actor=groups[g][p];
    if(!actor || !dBodyIsSet(actor) || seen.Find(actor)>=0 || receivers.Find(actor)>=0)
    {Failure="PART_IDENTITY_OR_PHYSICS";return false;}
    seen.Insert(actor);
   }
  }
  Expected=count*(count - 1)/2*24;
  int createdThisInstall;
  for(int first=0;first<count;first++)
  for(int second=first+1;second<count;second++)
  for(int a=0;a<5;a++)
  for(int b=0;b<5;b++)
  {
   if(a==0 && b==0){AuditLeaderPairsSkipped++;continue;}
   if(forceFailureAfter>=0 && createdThisInstall>=forceFailureAfter)
   {Failure="INJECTED_SETUP_FAILURE";Release();return false;}
   Animal_DGNativeProbe from=groups[first][a];Animal_DGNativeProbe to=groups[second][b];
   dBlock handle=dBodyCollisionBlock(from,to);
   if(!handle){Failure="NATIVE_BLOCK_FAILED";Release();return false;}
   NFSelectivePeerBlockEntry createdEntry=new NFSelectivePeerBlockEntry;
   createdEntry.Handle=handle;createdEntry.Owner=from;Entries.Insert(createdEntry);CreatedTotal++;createdThisInstall++;
  }
  if(Entries.Count()!=Expected){Failure="CREATED_COUNT";Release();return false;}
  return true;
 }
}
