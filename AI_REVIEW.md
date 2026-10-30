> 최신 반복 회귀(2026-10-31): B93D 예산 복원 ON은 8명·64명 각각 두 번 기능 게이트 통과. 64명 최종 도착 64/64 두 번, 낙하 복귀 83/81회. p95 66/60ms, 시각·성능·전체 골 미수용. [여섯 실행의 결과와 한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/touchdown-budget-six-20261031.md). 아래는 과거 보존 기록이다.

> 최신 실제8→64비교(2026-10-31): B93D의예산복원OFF/ON각300초를종료하고도착·낙하복귀와실제예산잔류표본을분리했다.후보추가반복·시각·성능수용은아직미완료다. [네실행의결과와한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/touchdown-budget-four-20261031.md). 아래는과거보존기록이다.

> 최신 실제8명비교(2026-10-31): B93D의예산복원OFF/ON각300초를종료하고도착·낙하복귀와실제예산잔류표본을분리했다.64명·추가반복·시각·성능수용은아직미완료다. [두실행의결과와한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/touchdown-budget-eight-20261031.md). 아래는과거보존기록이다.

> 최신 후보 패키지(2026-10-31): B93D 착지예산복원후보빌드·서명·검증성공. 실제PBO1,288행대조와실플레이씬컴파일통과. 기본OFF·원작업본보존. 새8/64·시각·성능은미수용이다. [검증범위와다음시험](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/touchdown-budget-package-20261031.md). 아래는과거보존기록이다.

> 최신 진단(2026-10-31): 동결 정체 입력에서 예산 복원8조건이 실제착지까지진행했고,정상낙하16조건은변화가없었다. 추가상태경계28행반복통과. 합성시간·동결포즈시험이며신규PBO·실제8/64·화면·성능은미수용이다. [조건과미확인범위](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/actor12-continuous-fall-20261031.md). 아래는과거보존기록이다.

> 최신 진단(2026-10-31): 12번 낙하 정체의 관절 충돌·예산 소진을 재현했고,착지 복구의1.5m 예산이 다음 낙하에 남는 상태값 누출을 확인했다. 임무전용복원후보는반복상태검사통과,새PBO·8/64·화면·성능은미검증이다. [근거와미수용범위](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/actor12-fall-budget-20261031.md). 아래는과거보존기록이다.

> 최신 실제 비교(2026-10-31): 같은 E57B 후보의8/64 OFF·ON 각300초 결과를 기록했다. 실패·후보 호출·정상 낙하 복귀를 구분하며,반복 회귀·화면 품질·성능 수용은 미완료다. [실행별 결과와 한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/endpoint-bounded-runtime-20261031.md). 아래는 과거 보존 기록이다.

> 새 후보 패키지(2026-10-31): E57B 끝점 선택기 벡터 한도만 수정. 빌드·서명·검증 및 동결 대조216행 통과. 기본 작업본 보존,새8→64·시각·성능 수용은 미실행이다. [후보 범위와 검증](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/endpoint-bounded-package-20261031.md). 아래는 이전 실제 시험/진단 기록이다.

> 최신 진단(2026-10-31): 60번의 −Z 표면 법선 거부를 17조건×2회 재현. 같은 호출의 raw4/이전접촉4로 실제 Accept 출력도 일치했다. 기존 선택기의 수치 한도 후보는 6dt×2회 통과했지만 게임 PBO·새8→64에는 미반영이다. [원인·후보·한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/actor60-footprint-accept-20261031.md). 아래 실제 기능 수치는 기존 시험 기록이다.

> 최신 실제 시험(2026-10-31): 고지대 목표 유지 조건에서8/8·64/64고지대 및 최종 도착.64개체 종료 검사1건,낙하 복귀73건,p95 76ms.반복 기능·화면 품질·성능 미수용. [실행 조건과 실패 증거](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/held-high-eight-sixtyfour-20261031.md). 아래 과거 수치는 원본 이력이다.

> 시험 조건 정정(2026-10-31): 기존64개체 HIGH_HOLD의 실제 플레이어 high 유지 약29.5초. 실제 high 유지 중 high+FOLLOW60/64이며, 최종64/64 도착과 별도다. 신규 런타임·수정·시각/성능 수용 아님. [원시 위치 기반 재분석](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/high-stimulus-validity-20261031.md).

> 최신 실측: idle·walk 고정1개체 영상 진단. callback12.8–13.4Hz를 표시fps로 해석하지 말 것. CPU포즈는 대부분 매관측프레임 변화했고 0lag 화면증분과 대응했다. 느린진행은 재현, 해결은 미완료. [결과와 한계](reports/operations/idle-walk-visible-20261031.md)

> 최신 분석: GPU 직접 이식 경로 미확보. 공간 색인은 이미 존재하며 이웃 검색 분산은 격리 실험 후보다. 끊김 원인은 이번에 확정하지 않았다. [코드 비교](reports/operations/external-horde-source-review-20261031.md)

# DayZ NPC AI — 외부 세션 검토 자료

> 최신 운영 감사: 완료된 파일이동 사례로 네 원칙을 대조했다. 적용조항은 범위내 충족, 신규설계조항은 N/A. 전체범위·효율성은 미검증이며 새 NPC 실험 없음. [감사 결과](reports/operations-latest.md)

> 최신 운영 기록: 이전 자료6폴더를 바탕화면 검토함으로 이동했다. 실제 삭제·공간 회수는 미확인이다. 새 NPC 실험은 없다. [최신 작업](reports/operations-latest.md)

> 운영 변경: Steam 최소 작업본을 기존 경로에 세팅했다. 대용량 삭제는 미완료다. 아래 실험 시각·수치는 변경하지 않았다. [최신 작업 상태](reports/operations-latest.md)

Snapshot: snapshot-20261030T004117-escape-refinement-runtime-20261030

Signed escape refinement package;selected source and diagnostics,not full raw evidence.
F1 DGSubsetZZZZZZEscapeRefinement.c SHA256 76256733b5dd8f9b282fdc848005ec30072812845232e6f00b9e8a5fd8a0c332
F1:1: // Opt-in archive candidate. Preserve the original eight directions first.
F1:2: modded class DGBNativeTraversal
F1:3: {
F1:4:     bool ERRead;bool EREnabled;int ERPlans;int ERTargets;int ERSweeps;int ERSelected;
F1:5:     int ERRows;static int ERGlobalRows;
F1:6:     bool EROn()
F1:7:     {
F1:8:         if(!ERRead){EREnabled=FileExist("$profile:escape-direction-refinement.flag");ERRead=true;}
F1:9:         return EREnabled;
F1:10:     }
F1:11:     override bool PlanPressureEscape(float dt)
F1:12:     {
F1:13:         if(!EROn())return super.PlanPressureEscape(dt);
F1:14:         bool planned=super.PlanPressureEscape(dt);
F1:15:         if(planned || Phase!="FOLLOW" || !PoseClearanceActive || Active || EntryAligning)return planned;
F1:16:         bool refined=ERPlan(dt);ERTrace(refined);return refined;
F1:17:     }
F1:18:     bool ERPlan(float dt)
F1:19:     {
F1:20:         ERPlans++;
F1:21:         array<bool> blocked={false,false,false,false,false,false,false,false};
F1:22:         string savedLanding=LandingDiagnostic;string savedDiagnostic=PressureEscapeDiagnostic;
F1:23:         for(float distance=0.5;distance<=4;distance+=0.5)
F1:24:         for(int index=0;index<8;index++)
F1:25:         {
F1:26:             if(blocked[index])continue;
F1:27:             float angle=(22.5+45*index)*Math.DEG2RAD;
F1:28:             vector direction=Vector(Math.Sin(angle),0,Math.Cos(angle));
F1:29:             vector candidate=Body.Agent.Position+direction*distance;
F1:30:             vector floor,supported;ERTargets++;
F1:31:             if(!DGBGroundAgent.Floor(Body.Leader,candidate,floor))continue;
F1:32:             if(Fixture().Area(floor)!=Fixture().Area(Body.Agent.Position))continue;
F1:33:             if(Math.AbsFloat(floor[1] - Body.Agent.Position[1])>0.08)continue;
F1:34:             if(!LandingSpace(floor,supported))continue;
F1:35:             float lift;ref array<vector> points={};ERSweeps++;
F1:36:             if(!PressureEscapeStepClear(Body.Agent.Position+direction*Math.Min(0.04,dt*0.6),lift,points))
F1:37:             {PressureEscapeLastStepReject=PressureEscapeDiagnostic;blocked[index]=true;continue;}
F1:38:             EscapeNoExitAt=-1;PressureEscapeGoal=supported;PressureEscapeValid=true;
F1:39:             LandingDiagnostic=savedLanding;ERSelected++;return true;
F1:40:         }
F1:41:         LandingDiagnostic=savedLanding;PressureEscapeDiagnostic=savedDiagnostic;return false;
F1:42:     }
F1:43:     void ERTrace(bool selected)
F1:44:     {
F1:45:         if(ERRows>=128 || ERGlobalRows>=2048 || !FileExist("$profile:state-contract.flag"))return;
F1:46:         string path="$profile:native-escape-refinement.tsv";bool exists=FileExist(path);
F1:47:         FileHandle file=OpenFile(path,FileMode.APPEND);if(!file)return;
F1:48:         if(!exists)FPrintln(file,"sequence\tseconds\tid\tphase\tposition\tselected\tgoal\tplans\ttargets\tsweeps\taccepted\trequest\tnativeRequest");
F1:49:         ERRows++;ERGlobalRows++;
F1:50:         string row=ERGlobalRows.ToString()+"\t"+(Now()-Body.Director.Born)+"\t"+Body.Id+"\t"+Phase+"\t"+Body.Agent.Position;
F1:51:         row+="\t"+selected+"\t"+PressureEscapeGoal+"\t"+ERPlans+"\t"+ERTargets+"\t"+ERSweeps+"\t"+ERSelected;
F1:52:         row+="\t"+Body.Request+"\t"+Body.Leader.NativeRequest;
F1:53:         FPrintln(file,row);CloseFile(file);
F1:54:     }
F1:55: }
Selection: closed run summaries and first up to12 selected refinement plan rows per source order.
{
  "buildId": "2FB65EB1FD77E334F8A36CF7ED5E4F4F0439C6B8CDD56976ECBDB13C0F70D226",
  "summaries": [
    {
      "runId": "runtime-20261030-092351",
      "buildId": "2FB65EB1FD77E334F8A36CF7ED5E4F4F0439C6B8CDD56976ECBDB13C0F70D226",
      "count": 8,
      "durationSeconds": 300,
      "scenario": "activeReference",
      "terminalArrivals": 8,
      "missingHighFollow": [],
      "highHoldWindow": [
        45.025,
        104.902
      ],
      "fallsLandedAndResumed": 9,
      "climbEndFailures": 0,
      "admissionOutcomes": {
        "ARRIVED": 1
      },
      "egressViolations": [],
      "foregroundMissionFrameMs": {
        "samples": 14734,
        "p50": 20.0,
        "p95": 33.0,
        "p99": 34.0
      },
      "refinementTrace": {
        "status": "activation_unobserved",
        "rows": 0,
        "actors": 0,
        "selectedRows": 0
      },
      "measurementConditions": {
        "escapeRefinement": "on",
        "contactInputObserver": true,
        "entryPreparationContact": "on",
        "diagnosticLogging": true,
        "timingScope": "foreground mission dt;not PresentMon or visual animation cadence",
        "baselineComparison": false
      }
    },
    {
      "runId": "runtime-20261030-093001",
      "buildId": "2FB65EB1FD77E334F8A36CF7ED5E4F4F0439C6B8CDD56976ECBDB13C0F70D226",
      "count": 64,
      "durationSeconds": 300,
      "scenario": "activeReference",
      "terminalArrivals": 64,
      "missingHighFollow": [
        17,
        33,
        40
      ],
      "highHoldWindow": [
        45.058,
        104.964
      ],
      "fallsLandedAndResumed": 77,
      "climbEndFailures": 0,
      "admissionOutcomes": {
        "ARRIVED": 9,
        "EXPIRED": 1
      },
      "egressViolations": [],
      "foregroundMissionFrameMs": {
        "samples": 9786,
        "p50": 26.0,
        "p95": 64.0,
        "p99": 86.0
      },
      "refinementTrace": {
        "status": "activation_unobserved",
        "rows": 0,
        "actors": 0,
        "selectedRows": 0
      },
      "measurementConditions": {
        "escapeRefinement": "on",
        "contactInputObserver": true,
        "entryPreparationContact": "on",
        "diagnosticLogging": true,
        "timingScope": "foreground mission dt;not PresentMon or visual animation cadence",
        "baselineComparison": false
      }
    }
  ],
  "details": [
    {
      "runId": "runtime-20261030-092351",
      "unresolvedTerminalAgents": [],
      "failureLabels": {
        "CONTACT_SUPPORT_LOST": 1
      },
      "algorithmFailureEpisodes": {},
      "allPhysicalTransitionsMatched": true,
      "refinementTrace": {
        "verificationStatus": "activation_unobserved",
        "sourceRunId": "runtime-20261030-092351",
        "buildId": "2FB65EB1FD77E334F8A36CF7ED5E4F4F0439C6B8CDD56976ECBDB13C0F70D226",
        "mode": "on",
        "rows": 0,
        "actors": 0,
        "selectedRows": 0,
        "perActor": {},
        "representativeSelected": [],
        "visualAcceptance": false,
        "performanceAcceptance": false,
        "limitations": [
          "Selected goals are plan outputs,not root commits or proof of eventual arrival.",
          "128/body2048global cap;last counters are lower bounds,not final totals.",
          "No trace can mean no eligible call or unobserved IO failure;not proof of branch absence.",
          "Original retry timing includes supported-turn resets;not a guaranteed0.75s minimum interval.",
          "Logical guards are source/headless evidence;live trace directly records FOLLOW only.",
          "Plan/target/sweep counts are not CPU,GPU or wall-clock cost attribution."
        ]
      }
    },
    {
      "runId": "runtime-20261030-093001",
      "unresolvedTerminalAgents": [],
      "failureLabels": {
        "CONTACT_SUPPORT_LOST": 51,
        "EGRESS_STALL": 1
      },
      "algorithmFailureEpisodes": {
        "EGRESS_STALL": [
          {
            "runId": "runtime-20261030-093001",
            "entityId": 14,
            "eventOrdinal": 1203,
            "seconds": 87.279,
            "previousState": "EGRESS",
            "newState": "FOLLOW",
            "transitionReason": "EGRESS_STALL",
            "metadataEvent": false,
            "recoveryGenerationDerived": 1,
            "position": "<4515.811035, 339.243652, 10241.250000>",
            "contactSampleAge": 0.14399999999999125,
            "contactStatus": "PLAYER_CONTACT",
            "contactPhase": "EGRESS",
            "landingDiagnostic": "CLEAR",
            "nativeSameCallSequence": "1342",
            "nativeRecoveryGeneration": "1",
            "nativeContactStatus": "",
            "nativeMotionElapsedBeforeTransition": "0",
            "terminalOutcome": "ARRIVED"
          }
        ]
      },
      "allPhysicalTransitionsMatched": true,
      "refinementTrace": {
        "verificationStatus": "activation_unobserved",
        "sourceRunId": "runtime-20261030-093001",
        "buildId": "2FB65EB1FD77E334F8A36CF7ED5E4F4F0439C6B8CDD56976ECBDB13C0F70D226",
        "mode": "on",
        "rows": 0,
        "actors": 0,
        "selectedRows": 0,
        "perActor": {},
        "representativeSelected": [],
        "visualAcceptance": false,
        "performanceAcceptance": false,
        "limitations": [
          "Selected goals are plan outputs,not root commits or proof of eventual arrival.",
          "128/body2048global cap;last counters are lower bounds,not final totals.",
          "No trace can mean no eligible call or unobserved IO failure;not proof of branch absence.",
          "Original retry timing includes supported-turn resets;not a guaranteed0.75s minimum interval.",
          "Logical guards are source/headless evidence;live trace directly records FOLLOW only.",
          "Plan/target/sweep counts are not CPU,GPU or wall-clock cost attribution."
        ]
      }
    }
  ],
  "packagedModeSummaries": [
    {
      "mode": 0,
      "rows": 600,
      "movedSteps": 0,
      "displacementMetres": 0.0,
      "supported": true,
      "diagnosticHalfMetreReached": false,
      "maximum3DStepMetres": 0,
      "refinementPlans": 0,
      "refinementTargets": 0,
      "refinementSweeps": 0,
      "refinementSelected": 0
    },
    {
      "mode": 1,
      "rows": 29,
      "movedSteps": 17,
      "displacementMetres": 0.507013,
      "supported": true,
      "diagnosticHalfMetreReached": true,
      "maximum3DStepMetres": 0.030274245821144283,
      "refinementPlans": 1,
      "refinementTargets": 4,
      "refinementSweeps": 2,
      "refinementSelected": 1
    },
    {
      "mode": 2,
      "rows": 600,
      "movedSteps": 0,
      "displacementMetres": 0.0,
      "supported": true,
      "diagnosticHalfMetreReached": false,
      "maximum3DStepMetres": 0,
      "refinementPlans": 0,
      "refinementTargets": 0,
      "refinementSweeps": 0,
      "refinementSelected": 0
    }
  ],
  "positiveFileReadModes": [
    {
      "mode": 0,
      "rows": 600,
      "movedSteps": 0,
      "displacementMetres": 0.0,
      "supported": true,
      "diagnosticHalfMetreReached": false,
      "maximum3DStepMetres": 0,
      "refinementPlans": 0,
      "refinementTargets": 0,
      "refinementSweeps": 0,
      "refinementSelected": 0
    },
    {
      "mode": 1,
      "rows": 29,
      "movedSteps": 17,
      "displacementMetres": 0.507013,
      "supported": true,
      "diagnosticHalfMetreReached": true,
      "maximum3DStepMetres": 0.030274245821144283,
      "refinementPlans": 1,
      "refinementTargets": 4,
      "refinementSweeps": 2,
      "refinementSelected": 1
    },
    {
      "mode": 2,
      "rows": 29,
      "movedSteps": 17,
      "displacementMetres": 0.507013,
      "supported": true,
      "diagnosticHalfMetreReached": true,
      "maximum3DStepMetres": 0.030274245821144283,
      "refinementPlans": 1,
      "refinementTargets": 4,
      "refinementSweeps": 2,
      "refinementSelected": 1
    }
  ],
  "negativeGuards": [
    {
      "case": "7",
      "mode": "0",
      "phase": "WALL",
      "poseActive": "true",
      "entryAlign": "false",
      "active": "false",
      "planned": "false",
      "erPlans": "0",
      "goal": "0 0 0",
      "valid": "false",
      "rootUnchanged": "true",
      "gate": "DEEPER_FIXTURE"
    },
    {
      "case": "8",
      "mode": "0",
      "phase": "FOOTPRINT",
      "poseActive": "true",
      "entryAlign": "false",
      "active": "false",
      "planned": "false",
      "erPlans": "0",
      "goal": "0 0 0",
      "valid": "false",
      "rootUnchanged": "true",
      "gate": "FOOT_Z_SUPPORT"
    }
  ]
}
Not proved: causal performance gain,visual animation smoothness,repeatable full acceptance from one live run per condition.

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
