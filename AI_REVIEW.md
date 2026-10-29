# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261029T131208-entry-preparation-policy-20261029

등반 준비 접촉 후보 — 경계·기하 통과, 실제8 OFF만 검증

작성: 2026-10-29T22:11:45.232625+09:00

1. 작업 목표

등반 직전 원래 120ms 준비 구간의 정렬을 뒤따르는 접촉 밀림이 무너뜨리는 문제에 한정해 후보를 검증한다. 정상 밀침·지지 상실 낙하·복구와 기존 실행 계층을 보존한다. 애니메이션/성능 목표는 별도로 유지한다.

2. 실제 수행

711C 후보를 구현·빌드·서명·검증했다. 기존11개 script는 바이트 동일하고 새12번째 모듈만 추가했다. 기본OFF다. unreserved CLIMB, FOLLOW, 유효한 같은 admission, fresh ENTRY_POSE_SETTLE, idle0/0, age<0.12초, 입구 반경0.36m·법선0.065m·yaw1도 이내에서만 접촉 변위를 벽 접선으로 투영한다. 새로운 물리/본 query나 직접 transform은 없다. 실행은 기존 Displace의 충돌·발판·지지 상실 검사를 그대로 거친다.

정책26개와 실제 inherited 이동/충돌/낙하8개를 엔진에서 확인했다. 첫 컴파일 실패들과 authored edge의 잘못된 낙하 fixture 결과를 모두 보존했다. rim-typed는 바닥 보정 실패 직후 native access violation이 발생했다. 보정 실패 시 정리 누락이 원인일 가능성은 있으나 단독 분리하지 않아 확정하지 않는다. 지원되는 시작점과 4cm/6cm 바깥 미지원점을 직접 찾고 실패 시 정리하는 rim-pair fixture는 정상 통과했다. PBO 자체는 이 과정에서 변경하지 않았다.

실제8명300초 OFF 기준판을 실행·정상 종료했다. ON 및64명은 아직 실행하지 않았다. 다른 승인 작업에서 화면 양보 요청이 들어왔고 OS 여유RAM941996KiB를 확인해 다음 클라이언트 실행을 미뤘다. 다른 프로그램/서버는 변경하지 않았다.

3. 측정 결과

정책26/26, 실제 기하8/8 통과. 접선 지지 상실은 OFF/ON 모두 FALL/CONTACT_FALL, 벽은 두 조건 모두 WORLD 거부였다. 아래 JSON은 실제OFF 한 실행의 결과다. 프레임 수치는 mission dt이며 화면 표시율/GPU Present가 아니다. 다른 작업 부하가 섞여 성능 비교에 사용할 수 없다.

json
{
  "runId": "runtime-20261029-220337",
  "count": 8,
  "flag": "off",
  "terminalArrivals": 8,
  "missingHighFollow": [],
  "fallsResumed": 10,
  "failureLabels": {
    "CONTACT_SUPPORT_LOST": 2
  },
  "ownerOutcomes": {
    "VALIDATED_FOLLOW": 7
  },
  "helperFailures": {},
  "ownerViolations": [],
  "gate": {
    "callGroups": 444,
    "climbNormalGateComparisons": 34,
    "normalBandLost65mmDuringPressure": 10,
    "normalBandLostWhileHeadingAligned": 1,
    "entryHeadingWorsenedDuringMotor": 10,
    "nearEntryApproachBearingConflict": 10,
    "unknownActiveLinks": {}
  },
  "foregroundCadence": {
    "true": 10373,
    "false": 899
  },
  "missionFrameMs": {
    "samples": 10170,
    "p50": 21.0,
    "p95": 35.0,
    "p99": 48.0
  },
  "callbackClock": {
    "observedMissionSeconds": 238.8610000000009,
    "nativeAdvanceSeconds": 181.15599999999998,
    "calls": 3623,
    "nativeClockPerMissionSecond": 0.7584159825170258,
    "callbackCallsPerMissionSecond": 15.167817266108685,
    "nativeAdvancePerCallback": 0.050001656086116474
  }
}


4. 원인 분석

사실: 정책 경계와 기존 충돌/정상 낙하 보존을 시험 범위에서 확인했고 실제OFF 실행에서 후보 계측이 생성됐다. OFF는 원래 입력을 그대로 전달한다. 가설: 준비 구간 정렬 유지가 실제 진입 지연/고지대 미도달을 줄일 수 있다. 미검증: ON 실제 군집 효과,64 회귀, 반복 비교, 실제 표시 애니메이션 개선. 반증된 시험 가정: authored 경계 또는 첫 바닥 미지원 샘플만으로 실제4cm 이동 끝이 낙하점이라고 볼 수 없었다. fixture를 수정했으며 기존 정책을 완화해 통과시키지 않았다.

5. 검증 수준

정책/기하: 진단 성공. 실제OFF: 정상 종료와 개별 결과는 위 수치. 후보ON: 미실행.8→64 회귀: 미완료. 반복 검증 완료/최종 수용: 아님. CPU callback clock과 보이는 애니메이션 시간은 동일하다고 간주하지 않는다. 시각/성능 개선 미수용,20ms p95는 미달성 목표다.

6. 변경 사항

subset_entry_preparation_contact.c를 추가하고 빌더에 연결했다. launcher의 별도 runtime OFF/ON 옵션과 양쪽 공통 detail trace를 추가했다. 기존 audit의 expected-build를 명시할 수 있게 했으며 기존47A7 기본값을 유지했다. 새 실행 trace 검증기와 단일요인 비교기를 준비했다. source-delta/packed source/signature 증거는 candidate-711C에 보존했다. 안전 종료 후 운영 기준 D61D 서명본으로 복원·재검증했다. 후보 source는 남아 있으나 현재 로드되는 PBO는 D61D다.

7. 원시 증거

실행ID runtime-20261029-220337. 각 validation의 실패/성공 engine-compile, profile 로그,26정책·8기하 TSV, 실제OFF state/contact/clock/entry TSV와 분석 JSON을 해시 검증해 아래 목록으로 연결한다. 기존 보고서/로그는 덮어쓰지 않았다. CUA 실제 장면은 한 시점 관찰이며 영상이나 시간적 부드러움 측정이 아니다. 공개 패킷은 선정 함수/대표 행만 제공하고 로컬 raw 전체·경로·키·게임 자산을 공개하지 않는다.

8. 잔여 문제

자원/화면 사용이 안정되면 동일711C와 같은 계측으로8 OFF/ON을 비교하고64 OFF/ON 및 반복 검증으로 진행한다. 이번OFF의 자원 간섭 때문에 필요하면 기준판부터 다시 수집한다. 단일 실행의 좋은 수치만으로 최적화 성공을 선언하지 않는다. FOLLOW 목표방위와 진입 정렬 간 간섭은 별도 후보로 남긴다. CPU 포즈 대 실제 표시 시간과64 비용 기여도는 별도 실험이다. 정상 낙하를 실패로 제거하지 않는다.

원본 보고서 ID: 2026-10-29_221145_232625+0900_npc-entry-preparation-policy.md
원본 SHA-256: 7c8f48a8561a44cc76daa3bf2906c36d86eb7668afe7cd66ac57398bf6bc7d73
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

Selection: policy26/all8 geometry;singleOFF aggregates;notallrawlogs.
{
  "runId": "runtime-20261029-220337",
  "count": 8,
  "flag": "off",
  "terminalArrivals": 8,
  "missingHighFollow": [],
  "fallsResumed": 10,
  "failureLabels": {
    "CONTACT_SUPPORT_LOST": 2
  },
  "ownerOutcomes": {
    "VALIDATED_FOLLOW": 7
  },
  "helperFailures": {},
  "ownerViolations": [],
  "gate": {
    "callGroups": 444,
    "climbNormalGateComparisons": 34,
    "normalBandLost65mmDuringPressure": 10,
    "normalBandLostWhileHeadingAligned": 1,
    "entryHeadingWorsenedDuringMotor": 10,
    "nearEntryApproachBearingConflict": 10,
    "unknownActiveLinks": {}
  },
  "foregroundCadence": {
    "true": 10373,
    "false": 899
  },
  "missionFrameMs": {
    "samples": 10170,
    "p50": 21.0,
    "p95": 35.0,
    "p99": 48.0
  },
  "callbackClock": {
    "observedMissionSeconds": 238.8610000000009,
    "nativeAdvanceSeconds": 181.15599999999998,
    "calls": 3623,
    "nativeClockPerMissionSecond": 0.7584159825170258,
    "callbackCallsPerMissionSecond": 15.167817266108685,
    "nativeAdvancePerCallback": 0.050001656086116474
  },
  "preparation": {
    "schemaVersion": 1,
    "sourceRunId": "runtime-20261029-220337",
    "buildId": "711CA8DFD0AFFB8B98B89C5181B1227781C3B5CD87F7DE1A0941AEAD211433E1",
    "count": 8,
    "enabled": false,
    "verificationStatus": "sampled_execution_verified",
    "sampledEligibleCalls": 13,
    "sampledChangedCalls": 0,
    "sampledAcceptedChangedCalls": 0,
    "sampledActors": {
      "0": 1,
      "3": 1,
      "6": 1,
      "5": 4,
      "7": 2,
      "4": 2,
      "2": 2
    },
    "sampledEntryTraceLinks": 1,
    "sampledRejectedReasons": {},
    "firstSeconds": 11.417,
    "lastSeconds": 50.794,
    "limitations": [
      "Eligible calls only, throttled to one per actor per 0.2 seconds; not all contacts.",
      "Post-call trace confirms executed proposal, not every pre-call selection gate or safe terminal outcome.",
      "No direct animation display, timing improvement or performance attribution from these rows."
    ],
    "fullGoalAccepted": false,
    "visualQualityAccepted": false,
    "performanceAccepted": false
  }
}

Experimental selection and execution source:
// Experimental local motor consistency during the existing <120ms idle prep.
// No peer reservations or direct transforms. Every executed proposal still
// delegates to the complete existing Displace chain, including support loss.
class DGEntryPreparationContact
{
    static bool Filter(DGBNativeGroundBody body,vector input,float dt,out vector projected,out string gate)
    {
        projected=input;gate="BODY";
        if(!body || !body.Director || !body.Traversal || !body.Leader){return false;}
        gate="OPEN_TRAVERSAL";
        if(!body.Director.OpenTraversal){return false;}
        gate="PHASE";
        if(body.Traversal.Phase!="FOLLOW" || body.Agent.State=="DEAD"){return false;}
        gate="OWNER";
        if(body.Traversal.IdleClearing || body.Traversal.SurfaceYieldActive || body.Traversal.PoseClearanceActive || body.Traversal.FRActive || body.Traversal.ARActive || body.Traversal.PTActive){return false;}
        if(body.YieldActive || body.SupportBypassActive || body.PeerBypassActive){return false;}
        if(body.Combat && (body.Combat.Active || body.Combat.Dead || body.Combat.PendingDeath)){return false;}
        gate="INTENT";
        if(!body.Traversal.Active || !body.Traversal.Admission || body.Traversal.AdmissionSource!=body.Traversal.Active){return false;}
        if(body.Traversal.Admission.Action!="CLIMB" || body.Traversal.Admission.SourceArea!=body.Traversal.Area){return false;}
        gate="PREPARING";
        if(!body.Traversal.EntryAligning || body.Reason!="ENTRY_POSE_SETTLE"){return false;}
        gate="IDLE_REQUESTS";
        if(body.Request!=0 || body.Leader.NativeRequest!=0){return false;}
        gate="AGE";
        float age=body.Director.Now-body.RequestedAt;
        if(age<0 || age>=0.12){return false;}
        gate="INPUT";
        if(dt<=0 || Math.AbsFloat(input[1])>0.000001){return false;}
        gate="ENTRY_RADIUS";
        if(DGBGroundAgent.FlatDistance(body.Agent.Position,body.Traversal.Admission.Entry)>0.36){return false;}
        float heading=body.Traversal.Admission.Heading;
        vector normal=DGBNativeGroundBody.Rotate("0 0 -1",heading);
        gate="NORMAL_GAP";
        if(Math.AbsFloat(vector.Dot(body.Agent.Position-body.Traversal.Admission.Entry,normal))>0.065){return false;}
        gate="HEADING";
        if(Math.AbsFloat(DGBGroundAgent.SignedAngle(body.Agent.Heading-heading))>1){return false;}
        vector tangent=DGBNativeGroundBody.Rotate("1 0 0",heading);
        projected=tangent*vector.Dot(input,tangent);gate="READY";return true;
    }
}

modded class DGBNativeCrowd
{
    bool EPCRead;bool EPCEnabled;bool EPCTrace;
    ref array<float> EPCNextLog={};
    int EPCCalls;int EPCEligible;int EPCChanged;int EPCAccepted;static int EPCSequence;
    override bool Displace(DGBNativeGroundBody body,vector displacement,float dt)
    {
        if(!EPCRead)
        {
            EPCEnabled=FileExist("$profile:entry-preparation-contact.flag");
            EPCTrace=FileExist("$profile:entry-preparation-detail.flag");EPCRead=true;
        }
        if(!EPCEnabled && !EPCTrace){return super.Displace(body,displacement,dt);}
        EPCCalls++;
        vector projected;string gate;
        bool eligible=DGEntryPreparationContact.Filter(body,displacement,dt,projected,gate);
        if(eligible){EPCEligible++;}
        vector executed=displacement;
        bool changed=EPCEnabled && eligible && vector.Distance(projected,displacement)>0.000001;
        if(changed){executed=projected;EPCChanged++;}
        vector before;float headingBefore;
        if(body){before=body.Agent.Position;headingBefore=body.Agent.Heading;}
        bool accepted=super.Displace(body,executed,dt);
        if(changed && accepted){EPCAccepted++;}
        if(EPCTrace && eligible){EPCLog(body,dt,displacement,projected,executed,before,headingBefore,changed,accepted,gate);}
        return accepted;
    }
    void EPCLog(DGBNativeGroundBody body,float dt,vector input,vector projected,vector executed,vector before,float headingBefore,bool changed,bool accepted,string gate)
    {
        if(!body || body.Id<0 || body.Id>255){return;}
        while(EPCNextLog.Count()<=body.Id){EPCNextLog.Insert(0);}
        if(Director.Now<EPCNextLog[body.Id]){return;}
        EPCNextLog[body.Id]=Director.Now+0.2;
        FileHandle file=OpenFile("$profile:native-entry-preparation.tsv",FileMode.APPEND);
        if(!file){Print("DG_ENTRY_PREPARATION_LOG_FAILED");return;}
        if(EPCSequence==0)
        {
            string header="sequence\tseconds\tid\tdisplaceCall\tenabled\tdt\tinput\tprojected\texecuted\tbefore\tafter\theadingBefore\theadingAfter";
            header+="\tchanged\taccepted\tgate\tphase\treason\trequestAge\trequest\tnativeRequest\tentryTraceCall\teligibleCalls\tchangedCalls\tacceptedChangedCalls";
            FPrintln(file,header);
        }
        int traceCall=-1;if(body.ETInside){traceCall=body.ETCallId;}
        EPCSequence++;
        string row=EPCSequence.ToString()+"\t"+(Director.Now-Director.Born)+"\t"+body.Id+"\t"+EPCCalls;
        row+="\t"+EPCEnabled+"\t"+dt+"\t"+input+"\t"+projected+"\t"+executed;
        row+="\t"+before+"\t"+body.Agent.Position+"\t"+headingBefore+"\t"+body.Agent.Heading;
        row+="\t"+changed+"\t"+accepted+"\t"+gate+"\t"+body.Traversal.Phase;
        row+="\t"+body.Reason+"\t"+(Director.Now-body.RequestedAt)+"\t"+body.Request+"\t"+body.Leader.NativeRequest;
        row+="\t"+traceCall+"\t"+EPCEligible+"\t"+EPCChanged+"\t"+EPCAccepted;
        FPrintln(file,row);CloseFile(file);
    }
}

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
