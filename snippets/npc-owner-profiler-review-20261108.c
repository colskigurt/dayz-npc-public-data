// Review-only mission instrumentation. No launcher/security changes.
bool NFProfileStarted;
bool NFProfileDone;
bool NFProfileEnabledAtStart;
float NFProfileStart;
int NFProfileFrames;

void NFWriteOwner(FileHandle output, string methodName, typename ownerType)
{
    int calls = EnProfiler.GetCountOfFunc(methodName, ownerType, true);
    float rawTime = EnProfiler.GetTimeOfFunc(methodName, ownerType, true);
    FPrintln(output, ownerType.ToString() + "::" + methodName + "|" + calls + "|" + rawTime);
}

void NFProfileUpdate()
{
#ifdef DIAG
    if (NFProfileDone) return;
    if (!NFProfileStarted && Age - RunAt >= 30)
    {
        EnProfiler.SetModule(EnProfilerModule.CORE);
        EnProfiler.SetFlags(EnProfilerFlags.RECURSIVE);
        EnProfiler.Enable(true, true);
        NFProfileStarted = true;
        NFProfileStart = Age;
        NFProfileEnabledAtStart = EnProfiler.IsEnabledC();
        Print("NF_NPC_PROFILE_BEGIN enabled=" + NFProfileEnabledAtStart + " age=" + Age);
    }
    if (!NFProfileStarted || Age - NFProfileStart < 30) return;
    EnProfiler.Enable(false, true);
    EnProfiler.SortData();
    array<ref EnProfilerTimeFuncPair> times = {};
    array<ref EnProfilerCountFuncPair> counts = {};
    EnProfiler.GetTimePerFunc(times, 60);
    EnProfiler.GetCountPerFunc(counts, 60);
    FileHandle timeFile = OpenFile("$profile:npc-function-times.txt", FileMode.WRITE);
    if (timeFile)
    {
        FPrintln(timeFile, "rawTime|function");
        foreach (EnProfilerTimeFuncPair timePair: times)
            FPrintln(timeFile, timePair.param1.ToString() + "|" + timePair.param2);
        CloseFile(timeFile);
    }
    FileHandle countFile = OpenFile("$profile:npc-function-counts.txt", FileMode.WRITE);
    if (countFile)
    {
        FPrintln(countFile, "count|function");
        foreach (EnProfilerCountFuncPair countPair: counts)
            FPrintln(countFile, countPair.param1.ToString() + "|" + countPair.param2);
        CloseFile(countFile);
    }
    FileHandle owners = OpenFile("$profile:npc-selected-owners.txt", FileMode.WRITE);
    if (owners)
    {
        FPrintln(owners, "function|count|rawTime");
        NFWriteOwner(owners, "SolveContactsInternal", StaticGetType(DGBNativeGroundBody));
        NFWriteOwner(owners, "Drive", StaticGetType(DGBNativeGroundBody));
        NFWriteOwner(owners, "Update", StaticGetType(DGBNativeGroundDirector));
        NFWriteOwner(owners, "Update", StaticGetType(DGBNativeCrowd));
        NFWriteOwner(owners, "RunPrePhys", StaticGetType(DGNativeCommand));
        CloseFile(owners);
    }
    FileHandle summary = OpenFile("$profile:npc-profiler-window.txt", FileMode.WRITE);
    if (summary)
    {
        FPrintln(summary, "startAge|endAge|enabledAtStart|profiledFrames|timeRows|countRows");
        FPrintln(summary, NFProfileStart.ToString() + "|" + Age + "|" + NFProfileEnabledAtStart + "|" + EnProfiler.GetProfiledSessionFrames() + "|" + times.Count() + "|" + counts.Count());
        CloseFile(summary);
    }
    NFProfileDone = true;
    Print("NF_NPC_PROFILE_DONE timeRows=" + times.Count() + " countRows=" + counts.Count());
#else
    NFProfileDone = true;
    Print("DG_ROLE_FAILED profiler_requires_diag_build");
#endif
}
