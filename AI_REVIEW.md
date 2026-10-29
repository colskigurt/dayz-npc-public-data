# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261029T061052-ground-handoff20-guard-20261029

지면 출구 검사 누락 재현 — guard는 작동했지만 복구0/3으로 후보 미채택

작성: 2026-10-29T15:09:58.971281+09:00

1. 작업 목표

runtime-20261029-143221에서 최종 미도달한20번의 ground EGRESS→FOLLOW 전환을 재현하고 정상 바닥 확인과 전체 이동 가능 상태를 구분한다. 검사 누락 방지와 실제 복구 성공을 별개로 판정한다. 정상 지붕 handoff와 압력 낙하를 유지한다.

2. 실제 수행

129.342·130.643·299.491초의 근처 기록 포즈3개와 장애물이 없는 지면 대조군을 별도 엔진 프로브에서 사용했다. 원래 handoff130.142초의 정확한 포즈는 아니며 정적 재현 한계를 유지한다. 본 월드 변환과 실제 SolveContactsInternal의 raw getter만 기록 포즈로 대체했고 역변환 원문·배포 PBO 멤버 일치를 증명했다. 기존 모든 접촉 게이트는 남겼다. native DriveGround는 충돌 거부 stub으로 명시했으므로 실제 AI/군집 재생은 아니다.

프로브의 긴 문자열 표현식 컴파일 오류와 actor binding 누락에 따른 VM 예외를 각각 보존했다. 표현식 분할과 명시적인 actor binding 후 별도 validation에서 통과했다. 이 두 실패는 NPC 클라이언트 런타임 충돌 증거가 아니며 결과 수용에서 제외했다. 실패 프로브가 PASS 문자열을 출력해도 오류 스캔이 거부한 사실을 유지한다.

ground-handoff-contact.flag AND footprint-recovery.flag가 있을 때만 nonwide ground 완료에도 기존 landing·area·fixture pose·planning inflation·SolveContacts 검사를 적용하는 후보를 구현했다. 거부 시 기존 bounded helper가 회복 목표를 유지하고 거리/시간 예산을 보존하도록 했다. 기본 OFF,지붕 handoff·경사 조건·낙하·애니메이션 속도는 변경하지 않았다.31A286 PBO를 빌드·서명·검증하고8/8 source member 일치와 엔진 컴파일을 확인했다.

이후 동일 포즈에서 flag OFF/ON을 비교했다. 실제 복구가 안 돼 기본 OFF를 유지했다.16방향×1/2/3cm 이동 및 ±1도 회전의150개 질의,±Z방향×1/3cm 직선 경로12개와 상세 충돌 기록을 추가했다. 새8/64 클라이언트 시험은 실행하지 않았다.

3. 측정 결과

|항목|결과|
|---|---|
|기존 ground 완료|포즈3개 모두 전체 pose/planning 실패 상태에서 FOLLOW 전환|
|root landing / SolveContacts|3개 모두 true|
|fixture endpoint / planning inflation|3개 모두 false|
|안전한 지면 대조군|전체 검사 true,정상 FOLLOW 전환|
|후보 ON 즉시 잘못된 완료 방지|3/3|
|후보 ON 실제 복구|0/3|
|후보 ON 실패 경계|3개 모두 기존 약5.05초에서 EGRESS_STALL|
|미세 이동/회전 질의|150개 고유 행|
|1cm 이동이 가능한 포즈|3/3,전체 경로 성공을 의미하지 않음|
|1/3cm 직선 경로 전체 복구|0/12|
|직선 마지막 거부 대상|12/12 StaticObj_Container_1C|

150개 질의의 허용 이동/회전 수는 포즈0=4/0,포즈1=4/1,포즈2=1/0이었다.12개 직선 사례는 최대 약0.030274m 이동 후 거부됐고,거부된 step은 root를 움직이지 않았다. 매 허용 step<=0.04m와 누적<=1.5m를 검증했다. 작은 step만으로 안전한 출구에 도달한다는 가설은 이 사례들에서 반증됐다.

상세 마지막 거부는 고정 root/torso probe1 또는2, hips probe3의 컨테이너 접촉이다. foot/terrain 거부가 아니다. 예:포즈0,+Z3cm 두 번째 step은 probe2, fraction0, normal=(-0.976872,-0.014046,-0.213364).포즈0,-Z3cm 첫 step은 probe1,fraction0.634845로 실제 이동 도중의 새 접촉을 기록했다. 초기 겹침과 중간 충돌을 무조건 같은 것으로 처리하면 안 된다.

이번 8/64 안정성·p50/p95/p99·실제 표시 갱신은 미실행/미측정이다. 이전63/64·고지대14명 미도달·p95120ms는 이전 별도 실행 결과이며 이번 후보의 값이나 개선으로 갱신하지 않는다.20ms는 계속 목표다.

4. 원인 분석

확인:바닥과 SolveContacts 통과만으로 fixture의 보수적 포즈 공간과 ground route의 여유 영역이 확보되지는 않는다. 기존 nonwide ground 완료 경로가 이를 확인하지 않아 충돌/경로 제약이 남은 위치에서 소유권을 해제할 수 있다. 이미 전체 검사가 있던 wide recovery와의 검사 범위 차이를 재현했다.

확인:후보는 이 잘못된 성공 전환을 막았지만 기존 ECPlan이 선택할 수 있는 전체 경로를 찾지 못해 물리 복구0/3이었다. 실패 라벨을 새로 보이게 한 것을 복구 성공으로 보고하지 않는다. 근처1cm step의 존재는 전체 출구 경로의 존재와 다르다.

반증:직선 step 크기 축소만으로 복구된다는 가설은0/12였다. 지면 아래 방향/발 terrain exit 조건 때문에 이번 직선들이 막혔다는 가설도 상세12개가 전부 컨테이너의 torso/hips 접촉으로 거부돼 반증됐다. 해당 지면 조건을 완화하는 수정은 하지 않았다.

추정/미확정:이동·회전 조합의 휘어진 경로,다른 실제 애니메이션 포즈,또는 착지 전 위치 선택에는 안전한 해가 있을 수 있다. 이번 고정 포즈/직선 실험은 모든 경로의 불가능성을 입증하지 않는다. 초기 착지에서 회복하기 어려운 좁은 공간을 허용한 경로도 별도 확인이 필요하다. 동결 포즈와 명시적인 walk stub 때문에 라이브 군집의 복구 효과는 미검증이다.

5. 검증 수준

진단 성공:전체 footprint가 미확보인 ground handoff 재현과 직접 거부 대상 구분. 도구/한정 계약 검사 통과:기본 OFF 재현,안전한 지면 대조군,거부 step 불변·거리 예산. 후보 미채택:0/3 복구. 반복 검증 완료:일반 실제 군집 회귀·복구 성공에는 해당 없음. 실제8/64 회귀:미실행. 최종 수용:기능·시각·성능 모두 미수용이다.

6. 변경 사항

subset_motion_contracts.c에 opt-in ground 완료 검사·회복 목표 유지·제한 로그를 추가했다. stage_horde_ground_handoff20.py와 별도 compile 선택/marker 검사를 추가했다. NPC 기본 flag는 OFF이며 전체 경사/접촉 검사를 완화하거나 순간 이동·새 보간을 적용하지 않았다.

현재 후보 빌드31A286A3BFC8D8857F70415E9489CD29EE321D0EF8569F3F613629E9868FD126의 서명 및 검증 성공.8개 source script와 packed member의 목록/바이트 일치.엔진 smoke 통과. 새 게임플레이/수정판 준비 완료를 뜻하지 않는다. 이전83E94 배포본·source·증명을 baseline-83E94에 같은 해시로 보존했고31A286 후보도 별도 보존했다. 개인 키는 포함하지 않는다. 운영 서버는 변경하지 않았다.

7. 원시 증거

baseline-replay-analysis,candidate-guard-analysis,local-first-step-analysis,corridor-analysis,corridor-detail-analysis는 각각 별도 프로브의 raw TSV·staged source·engine 결과 SHA에 연결된다. 실패한 initial/compact 프로브도 삭제하지 않았다. 저널의 유효하지 않은 out parameter는 해석에서 제외한다. 특히 local 회전의 fullEndpoint는 원래 heading을 사용하므로 회전 후 endpoint 증거로 쓰지 않고,probe<0의 depth 또는 floorFound=false의 normal도 사용하지 않는다.

보고서는 모든 관련 compiler PID가 종료됐고 파일/JSON/해시가 안정된 뒤 저장한다. 공개 패킷은 선정 수치·대표 거부와 자체 모드 함수만 포함하며 원시 로그 전체·프로세스·내부 경로·키·설치 자산은 공개하지 않는다. 과거 보고서가 가리키는 갱신 가능한 build/source 파일의 이전 바이트는 이번 baseline inventory에서 확인할 수 있다. 과거 보고서는 변경하지 않는다.

8. 잔여 문제

다음 기능 실험:기록된 pose에서 제한된 이동·회전 조합 경로가 존재하는지 검사하고,없으면 라이브 pose 변화 및 낙하/착지 단계에서 좁은 공간 진입을 방지할 수 있는지 계측한다. 새 world contact를 무시하거나 fixture를 통과시키는 복구는 채택하지 않는다. guard만으로 회복되지 않으므로 이를 기본 기능으로 승격하지 않는다. 실제 안전한 복구 후보가 확보되면 별도 동일 조건8→64 회귀와 반복을 진행한다.

애니메이션의 callback/표시 시간 차이,실제 visible-frame 관측과64명 비용 귀속은 여전히 별도 미완료다. 보안 창 때문에 막힌 UI 관찰을 성공으로 보고하지 않으며 임의 배속/보간을 적용하지 않는다. 현재 후보 PBO와 이전 runtime compile hash가 다르므로 새 클라이언트 시험 전에 별도 runtime validation이 필요하며 무결성 검사 우회를 하지 않는다.

원본 보고서 ID: 2026-10-29_150958_971281+0900_npc-ground-handoff20-guard.md
원본 SHA-256: 35ce05b0e8b27b4a548d4a88cf75e2e3f6937a61a93e224d50524294d343ccad
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

Selected derived evidence;not full runtime/visible evidence
{
  "baseline-replay-analysis.json": {
    "enginePassed": true,
    "rows": [
      {
        "snapshot": "0",
        "mode": "0",
        "iteration": "0",
        "seconds": "0.0500031",
        "previousPhase": "EGRESS",
        "phase": "FOLLOW",
        "before": "<4511.504395, 339.171906, 10236.788086>",
        "position": "<4511.504395, 339.171906, 10236.788086>",
        "contactBefore": "true",
        "contactReasonBefore": "",
        "contactAfter": "true",
        "contactReasonAfter": "NATIVE_JOINT_WORLD_REPLAY_STUB",
        "reason": "NATIVE_JOINT_WORLD_REPLAY_STUB",
        "failure": "",
        "wideRecovery": "false",
        "ecActive": "false",
        "ecDistance": "0",
        "landingBefore": "true",
        "endpointBefore": "false",
        "planningBefore": "false",
        "fullAfter": "false"
      },
      {
        "snapshot": "1",
        "mode": "0",
        "iteration": "0",
        "seconds": "0.0500031",
        "previousPhase": "EGRESS",
        "phase": "FOLLOW",
        "before": "<4511.504395, 339.171906, 10236.788086>",
        "position": "<4511.504395, 339.171906, 10236.788086>",
        "contactBefore": "true",
        "contactReasonBefore": "NATIVE_JOINT_WORLD_REPLAY_STUB",
        "contactAfter": "true",
        "contactReasonAfter": "NATIVE_JOINT_WORLD_REPLAY_STUB",
        "reason": "NATIVE_JOINT_WORLD_REPLAY_STUB",
        "failure": "",
        "wideRecovery": "false",
        "ecActive": "false",
        "ecDistance": "0",
        "landingBefore": "true",
        "endpointBefore": "false",
        "planningBefore": "false",
        "fullAfter": "false"
      },
      {
        "snapshot": "2",
        "mode": "0",
        "iteration": "0",
        "seconds": "0.0499878",
        "previousPhase": "EGRESS",
        "phase": "FOLLOW",
        "before": "<4511.504395, 339.171906, 10236.788086>",
        "position": "<4511.504395, 339.171906, 10236.788086>",
        "contactBefore": "true",
        "contactReasonBefore": "NATIVE_JOINT_WORLD_REPLAY_STUB",
        "contactAfter": "true",
        "contactReasonAfter": "NATIVE_JOINT_WORLD_REPLAY_STUB",
        "reason": "NATIVE_JOINT_WORLD_REPLAY_STUB",
        "failure": "",
        "wideRecovery": "false",
        "ecActive": "false",
        "ecDistance": "0",
        "landingBefore": "true",
        "endpointBefore": "false",
        "planningBefore": "false",
        "fullAfter": "false"
      },
      {
        "snapshot": "3",
        "mode": "0",
        "iteration": "0",
        "seconds": "0.0499878",
        "previousPhase": "EGRESS",
        "phase": "FOLLOW",
        "before": "<4530.000000, 339.336670, 10240.000000>",
        "position": "<4530.000000, 339.336670, 10240.000000>",
        "contactBefore": "true",
        "contactReasonBefore": "NATIVE_JOINT_WORLD_REPLAY_STUB",
        "contactAfter": "true",
        "contactReasonAfter": "NATIVE_JOINT_WORLD_REPLAY_STUB",
        "reason": "NATIVE_JOINT_WORLD_REPLAY_STUB",
        "failure": "",
        "wideRecovery": "false",
        "ecActive": "false",
        "ecDistance": "0",
        "landingBefore": "true",
        "endpointBefore": "true",
        "planningBefore": "true",
        "fullAfter": "true"
      }
    ],
    "prematureFullFootprintHandoffs": 3,
    "clearControlPassed": true,
    "scope": "Recorded nearby frozen poses, actual ground Egress with blocked-walk stub; not exact original handoff pose or live movement.",
    "conclusion": "Root support and SolveContacts can pass while fixture-pose footprint and planning inflation fail. Nonwide ground completion released ownership anyway.",
    "limitations": [
      "Initial formula compile rejection and unbound-actor VM probe preserved, not admitted.",
      "A blocked-walk reason alone does not prove final current contact failure.",
      "No visual/performance acceptance."
    ],
    "functionalAccepted": false
  },
  "candidate-guard-analysis.json": {
    "buildId": "31A286A3BFC8D8857F70415E9489CD29EE321D0EF8569F3F613629E9868FD126",
    "cases": [
      {
        "snapshot": 0,
        "mode": 0,
        "count": 1,
        "last": {
          "snapshot": "0",
          "mode": "0",
          "iteration": "0",
          "seconds": "0.0500031",
          "previousPhase": "EGRESS",
          "phase": "FOLLOW",
          "before": "<4511.504395, 339.171906, 10236.788086>",
          "position": "<4511.504395, 339.171906, 10236.788086>",
          "contactBefore": "true",
          "contactReasonBefore": "",
          "contactAfter": "true",
          "contactReasonAfter": "NATIVE_JOINT_WORLD_REPLAY_STUB",
          "reason": "NATIVE_JOINT_WORLD_REPLAY_STUB",
          "failure": "",
          "wideRecovery": "false",
          "ecActive": "false",
          "ecDistance": "0",
          "landingBefore": "true",
          "endpointBefore": "false",
          "planningBefore": "false",
          "fullAfter": "false"
        }
      },
      {
        "snapshot": 0,
        "mode": 1,
        "count": 101,
        "last": {
          "snapshot": "0",
          "mode": "1",
          "iteration": "100",
          "seconds": "5.05",
          "previousPhase": "EGRESS",
          "phase": "FOLLOW",
          "before": "<4511.504395, 339.171906, 10236.788086>",
          "position": "<4511.504395, 339.171906, 10236.788086>",
          "contactBefore": "true",
          "contactReasonBefore": "EGRESS_GROUND_HANDOFF_BLOCKED",
          "contactAfter": "true",
          "contactReasonAfter": "EGRESS_GROUND_HANDOFF_BLOCKED",
          "reason": "EGRESS_GROUND_HANDOFF_BLOCKED",
          "failure": "EGRESS_STALL",
          "wideRecovery": "false",
          "ecActive": "false",
          "ecDistance": "0",
          "landingBefore": "true",
          "endpointBefore": "false",
          "planningBefore": "false",
          "fullAfter": "false"
        }
      },
      {
        "snapshot": 1,
        "mode": 0,
        "count": 1,
        "last": {
          "snapshot": "1",
          "mode": "0",
          "iteration": "0",
          "seconds": "0.0500031",
          "previousPhase": "EGRESS",
          "phase": "FOLLOW",
          "before": "<4511.504395, 339.171906, 10236.788086>",
          "position": "<4511.504395, 339.171906, 10236.788086>",
          "contactBefore": "true",
          "contactReasonBefore": "EGRESS_GROUND_HANDOFF_BLOCKED",
          "contactAfter": "true",
          "contactReasonAfter": "NATIVE_JOINT_WORLD_REPLAY_STUB",
          "reason": "NATIVE_JOINT_WORLD_REPLAY_STUB",
          "failure": "",
          "wideRecovery": "false",
          "ecActive": "false",
          "ecDistance": "0",
          "landingBefore": "true",
          "endpointBefore": "false",
          "planningBefore": "false",
          "fullAfter": "false"
        }
      },
      {
        "snapshot": 1,
        "mode": 1,
        "count": 101,
        "last": {
          "snapshot": "1",
          "mode": "1",
          "iteration": "100",
          "seconds": "5.04999",
          "previousPhase": "EGRESS",
          "phase": "FOLLOW",
          "before": "<4511.504395, 339.171906, 10236.788086>",
          "position": "<4511.504395, 339.171906, 10236.788086>",
          "contactBefore": "true",
          "contactReasonBefore": "EGRESS_GROUND_HANDOFF_BLOCKED",
          "contactAfter": "true",
          "contactReasonAfter": "EGRESS_GROUND_HANDOFF_BLOCKED",
          "reason": "EGRESS_GROUND_HANDOFF_BLOCKED",
          "failure": "EGRESS_STALL",
          "wideRecovery": "false",
          "ecActive": "false",
          "ecDistance": "0",
          "landingBefore": "true",
          "endpointBefore": "false",
          "planningBefore": "false",
          "fullAfter": "false"
        }
      },
      {
        "snapshot": 2,
        "mode": 0,
        "count": 1,
        "last": {
          "snapshot": "2",
          "mode": "0",
          "iteration": "0",
          "seconds": "0.0499878",
          "previousPhase": "EGRESS",
          "phase": "FOLLOW",
          "before": "<4511.504395, 339.171906, 10236.788086>",
          "position": "<4511.504395, 339.171906, 10236.788086>",
          "contactBefore": "true",
          "contactReasonBefore": "EGRESS_GROUND_HANDOFF_BLOCKED",
          "contactAfter": "true",
          "contactReasonAfter": "NATIVE_JOINT_WORLD_REPLAY_STUB",
          "reason": "NATIVE_JOINT_WORLD_REPLAY_STUB",
          "failure": "",
          "wideRecovery": "false",
          "ecActive": "false",
          "ecDistance": "0",
          "landingBefore": "true",
          "endpointBefore": "false",
          "planningBefore": "false",
          "fullAfter": "false"
        }
      },
      {
        "snapshot": 2,
        "mode": 1,
        "count": 101,
        "last": {
          "snapshot": "2",
          "mode": "1",
          "iteration": "100",
          "seconds": "5.04999",
          "previousPhase": "EGRESS",
          "phase": "FOLLOW",
          "before": "<4511.504395, 339.171906, 10236.788086>",
          "position": "<4511.504395, 339.171906, 10236.788086>",
          "contactBefore": "true",
          "contactReasonBefore": "EGRESS_GROUND_HANDOFF_BLOCKED",
          "contactAfter": "true",
          "contactReasonAfter": "EGRESS_GROUND_HANDOFF_BLOCKED",
          "reason": "EGRESS_GROUND_HANDOFF_BLOCKED",
          "failure": "EGRESS_STALL",
          "wideRecovery": "false",
          "ecActive": "false",
          "ecDistance": "0",
          "landingBefore": "true",
          "endpointBefore": "false",
          "planningBefore": "false",
          "fullAfter": "false"
        }
      },
      {
        "snapshot": 3,
        "mode": 0,
        "count": 1,
        "last": {
          "snapshot": "3",
          "mode": "0",
          "iteration": "0",
          "seconds": "0.0499878",
          "previousPhase": "EGRESS",
          "phase": "FOLLOW",
          "before": "<4530.000000, 339.336670, 10240.000000>",
          "position": "<4530.000000, 339.336670, 10240.000000>",
          "contactBefore": "true",
          "contactReasonBefore": "EGRESS_GROUND_HANDOFF_BLOCKED",
          "contactAfter": "true",
          "contactReasonAfter": "NATIVE_JOINT_WORLD_REPLAY_STUB",
          "reason": "NATIVE_JOINT_WORLD_REPLAY_STUB",
          "failure": "",
          "wideRecovery": "false",
          "ecActive": "false",
          "ecDistance": "0",
          "landingBefore": "true",
          "endpointBefore": "true",
          "planningBefore": "true",
          "fullAfter": "true"
        }
      },
      {
        "snapshot": 3,
        "mode": 1,
        "count": 1,
        "last": {
          "snapshot": "3",
          "mode": "1",
          "iteration": "0",
          "seconds": "0.0499878",
          "previousPhase": "EGRESS",
          "phase": "FOLLOW",
          "before": "<4530.000000, 339.336670, 10240.000000>",
          "position": "<4530.000000, 339.336670, 10240.000000>",
          "contactBefore": "true",
          "contactReasonBefore": "NATIVE_JOINT_WORLD_REPLAY_STUB",
          "contactAfter": "true",
          "contactReasonAfter": "NATIVE_JOINT_WORLD_REPLAY_STUB",
          "reason": "NATIVE_JOINT_WORLD_REPLAY_STUB",
          "failure": "",
          "wideRecovery": "false",
          "ecActive": "false",
          "ecDistance": "0",
          "landingBefore": "true",
          "endpointBefore": "true",
          "planningBefore": "true",
          "fullAfter": "true"
        }
      }
    ],
    "unsafeImmediateHandoffPrevented": 3,
    "physicalRecoveries": 0,
    "explicitStallFailures": 3,
    "clearControlPassed": true,
    "candidateAccepted": false,
    "conclusion": "Guard corrects premature success but all3 frozen blocked poses remain immobile and expire at existing5sec bound. Not a recovery fix. Default flag remains OFF."
  },
  "local-first-step-analysis.json": {
    "enginePassed": true,
    "queryRows": 150,
    "snapshots": [
      {
        "snapshot": 0,
        "clearCounts": {
          "TRANSLATE": 4
        },
        "clearQueries": [
          {
            "query": "4",
            "kind": "TRANSLATE",
            "direction": "4",
            "distance": "0.01",
            "position": "<4511.504395, 339.171936, 10236.797852>",
            "heading": "7.8443",
            "diagnostic": "NONDEEPENING_CLEAR"
          },
          {
            "query": "12",
            "kind": "TRANSLATE",
            "direction": "12",
            "distance": "0.01",
            "position": "<4511.504395, 339.171844, 10236.778320>",
            "heading": "7.8443",
            "diagnostic": "NONDEEPENING_CLEAR"
          },
          {
            "query": "20",
            "kind": "TRANSLATE",
            "direction": "4",
            "distance": "0.02",
            "position": "<4511.504395, 339.171967, 10236.807617>",
            "heading": "7.8443",
            "diagnostic": "NONDEEPENING_CLEAR"
          },
          {
            "query": "36",
            "kind": "TRANSLATE",
            "direction": "4",
            "distance": "0.03",
            "position": "<4511.504395, 339.172028, 10236.818359>",
            "heading": "7.8443",
            "diagnostic": "NONDEEPENING_CLEAR"
          }
        ],
        "rejectCounts": {
          "NEW_WORLD_CONTACT": 38,
          "DEEPER_FIXTURE": 8
        }
      },
      {
        "snapshot": 1,
        "clearCounts": {
          "TRANSLATE": 4,
          "ROTATE": 1
        },
        "clearQueries": [
          {
            "query": "4",
            "kind": "TRANSLATE",
            "direction": "4",
            "distance": "0.01",
            "position": "<4511.504395, 339.171936, 10236.797852>",
            "heading": "4.8443",
            "diagnostic": "NONDEEPENING_CLEAR"
          },
          {
            "query": "12",
            "kind": "TRANSLATE",
            "direction": "12",
            "distance": "0.01",
            "position": "<4511.504395, 339.171844, 10236.778320>",
            "heading": "4.8443",
            "diagnostic": "NONDEEPENING_CLEAR"
          },
          {
            "query": "20",
            "kind": "TRANSLATE",
            "direction": "4",
            "distance": "0.02",
            "position": "<4511.504395, 339.171967, 10236.807617>",
            "heading": "4.8443",
            "diagnostic": "NONDEEPENING_CLEAR"
          },
          {
            "query": "36",
            "kind": "TRANSLATE",
            "direction": "4",
            "distance": "0.03",
            "position": "<4511.504395, 339.172028, 10236.818359>",
            "heading": "4.8443",
            "diagnostic": "NONDEEPENING_CLEAR"
          },
          {
            "query": "49",
            "kind": "ROTATE",
            "direction": "-1",
            "distance": "0",
            "position": "<4511.504395, 339.171906, 10236.788086>",
            "heading": "3.8443",
            "diagnostic": "NONDEEPENING_CLEAR"
          }
        ],
        "rejectCounts": {
          "NEW_WORLD_CONTACT": 38,
          "DEEPER_FIXTURE": 7
        }
      },
      {
        "snapshot": 2,
        "clearCounts": {
          "TRANSLATE": 1
        },
        "clearQueries": [
          {
            "query": "12",
            "kind": "TRANSLATE",
            "direction": "12",
            "distance": "0.01",
            "position": "<4511.504395, 339.171844, 10236.778320>",
            "heading": "-2.1557",
            "diagnostic": "NONDEEPENING_CLEAR"
          }
        ],
        "rejectCounts": {
          "NEW_WORLD_CONTACT": 45,
          "DEEPER_FIXTURE": 4
        }
      }
    ],
    "conclusion": "All3 poses have at least one legal1cm translation, despite no admitted long chord. Fine segmented corridor experiment justified; no runtime change yet.",
    "limitations": [
      "FullEndpoint column for ROTATE uses original body heading; do not use it as rotated-endpoint proof.",
      "Depth fields are valid only on rejected rows with nonnegative probe; unassigned values can retain earlier loop output.",
      "Small step feasibility is not a full recovery route or performance proof."
    ]
  },
  "corridor-analysis.json": {
    "cases": [
      {
        "snapshot": "0",
        "direction": "0",
        "stepSize": "0.03",
        "rows": 2,
        "acceptedSteps": 1,
        "travel": 0.0302737,
        "lastDiagnostic": "NEW_WORLD_CONTACT",
        "fullEndpoint": false
      },
      {
        "snapshot": "0",
        "direction": "0",
        "stepSize": "0.01",
        "rows": 4,
        "acceptedSteps": 3,
        "travel": 0.0292972,
        "lastDiagnostic": "NEW_WORLD_CONTACT",
        "fullEndpoint": false
      },
      {
        "snapshot": "0",
        "direction": "1",
        "stepSize": "0.03",
        "rows": 1,
        "acceptedSteps": 0,
        "travel": 0.0,
        "lastDiagnostic": "NEW_WORLD_CONTACT",
        "fullEndpoint": false
      },
      {
        "snapshot": "0",
        "direction": "1",
        "stepSize": "0.01",
        "rows": 3,
        "acceptedSteps": 2,
        "travel": 0.0195315,
        "lastDiagnostic": "NEW_WORLD_CONTACT",
        "fullEndpoint": false
      },
      {
        "snapshot": "1",
        "direction": "0",
        "stepSize": "0.03",
        "rows": 2,
        "acceptedSteps": 1,
        "travel": 0.0302737,
        "lastDiagnostic": "NEW_WORLD_CONTACT",
        "fullEndpoint": false
      },
      {
        "snapshot": "1",
        "direction": "0",
        "stepSize": "0.01",
        "rows": 4,
        "acceptedSteps": 3,
        "travel": 0.0292972,
        "lastDiagnostic": "NEW_WORLD_CONTACT",
        "fullEndpoint": false
      },
      {
        "snapshot": "1",
        "direction": "1",
        "stepSize": "0.03",
        "rows": 1,
        "acceptedSteps": 0,
        "travel": 0.0,
        "lastDiagnostic": "NEW_WORLD_CONTACT",
        "fullEndpoint": false
      },
      {
        "snapshot": "1",
        "direction": "1",
        "stepSize": "0.01",
        "rows": 3,
        "acceptedSteps": 2,
        "travel": 0.0195315,
        "lastDiagnostic": "NEW_WORLD_CONTACT",
        "fullEndpoint": false
      },
      {
        "snapshot": "2",
        "direction": "0",
        "stepSize": "0.03",
        "rows": 1,
        "acceptedSteps": 0,
        "travel": 0.0,
        "lastDiagnostic": "NEW_WORLD_CONTACT",
        "fullEndpoint": false
      },
      {
        "snapshot": "2",
        "direction": "0",
        "stepSize": "0.01",
        "rows": 1,
        "acceptedSteps": 0,
        "travel": 0.0,
        "lastDiagnostic": "NEW_WORLD_CONTACT",
        "fullEndpoint": false
      },
      {
        "snapshot": "2",
        "direction": "1",
        "stepSize": "0.03",
        "rows": 1,
        "acceptedSteps": 0,
        "travel": 0.0,
        "lastDiagnostic": "NEW_WORLD_CONTACT",
        "fullEndpoint": false
      },
      {
        "snapshot": "2",
        "direction": "1",
        "stepSize": "0.01",
        "rows": 3,
        "acceptedSteps": 2,
        "travel": 0.0195315,
        "lastDiagnostic": "NEW_WORLD_CONTACT",
        "fullEndpoint": false
      }
    ],
    "fullRecoveries": 0,
    "casesVerified": 12,
    "allRejectedStepsPreserveRoot": true,
    "maximumStepBound": 0.04,
    "maximumTravelBound": 1.5,
    "conclusion": "Fine straight1cm steps do not solve exit. No segmented runtime policy adopted.",
    "visualAccepted": false,
    "performanceAccepted": false
  },
  "corridor-detail-analysis.json": {
    "casesVerified": 12,
    "fullRecoveries": 0,
    "rejectedProbes": {
      "2": 4,
      "1": 6,
      "3": 2
    },
    "rejectedFixtures": {
      "StaticObj_Container_1C": 12
    },
    "rejectedRows": [
      {
        "snapshot": "0",
        "direction": "0",
        "stepSize": "0.03",
        "iteration": "1",
        "accepted": "false",
        "previous": "<4511.504395, 339.172028, 10236.818359>",
        "position": "<4511.504395, 339.172028, 10236.818359>",
        "lift": "0.0270691",
        "travel": "0.0302737",
        "fullEndpoint": "false",
        "diagnostic": "NEW_WORLD_CONTACT",
        "probe": "2",
        "fixture": "StaticObj_Container_1C",
        "fraction": "0",
        "normal": "<-0.976872, -0.014046, -0.213364>",
        "oldProbe": "<4511.504395, 340.672028, 10236.818359>",
        "newProbe": "<4511.504395, 340.672150, 10236.848633>",
        "radius": "0.23",
        "floorFound": "false",
        "floorObject": "NULL",
        "floorNormal": "<0.000000, 0.000000, 0.000000>",
        "newFootGap": "-10000",
        "probeYDelta": "0.00012207"
      },
      {
        "snapshot": "0",
        "direction": "0",
        "stepSize": "0.01",
        "iteration": "3",
        "accepted": "false",
        "previous": "<4511.504395, 339.172028, 10236.817383>",
        "position": "<4511.504395, 339.172028, 10236.817383>",
        "lift": "0.0270691",
        "travel": "0.0292972",
        "fullEndpoint": "false",
        "diagnostic": "NEW_WORLD_CONTACT",
        "probe": "2",
        "fixture": "StaticObj_Container_1C",
        "fraction": "0",
        "normal": "<-0.976113, -0.014030, -0.216810>",
        "oldProbe": "<4511.504395, 340.672028, 10236.817383>",
        "newProbe": "<4511.504395, 340.672058, 10236.827148>",
        "radius": "0.23",
        "floorFound": "false",
        "floorObject": "NULL",
        "floorNormal": "<0.000000, 0.000000, 0.000000>",
        "newFootGap": "-10000",
        "probeYDelta": "3.05176e-05"
      },
      {
        "snapshot": "0",
        "direction": "1",
        "stepSize": "0.03",
        "iteration": "0",
        "accepted": "false",
        "previous": "<4511.504395, 339.171906, 10236.788086>",
        "position": "<4511.504395, 339.171906, 10236.788086>",
        "lift": "0.0270691",
        "travel": "0",
        "fullEndpoint": "false",
        "diagnostic": "NEW_WORLD_CONTACT",
        "probe": "1",
        "fixture": "StaticObj_Container_1C",
        "fraction": "0.634845",
        "normal": "<0.862339, -0.438634, 0.252928>",
        "oldProbe": "<4511.504395, 340.071899, 10236.788086>",
        "newProbe": "<4511.504395, 340.071777, 10236.757813>",
        "radius": "0.23",
        "floorFound": "true",
        "floorObject": "",
        "floorNormal": "<-0.018661, 0.999818, -0.003999>",
        "newFootGap": "0.669983",
        "probeYDelta": "-0.00012207"
      },
      {
        "snapshot": "0",
        "direction": "1",
        "stepSize": "0.01",
        "iteration": "2",
        "accepted": "false",
        "previous": "<4511.504395, 339.171814, 10236.768555>",
        "position": "<4511.504395, 339.171814, 10236.768555>",
        "lift": "0.0270691",
        "travel": "0.0195315",
        "fullEndpoint": "false",
        "diagnostic": "NEW_WORLD_CONTACT",
        "probe": "1",
        "fixture": "StaticObj_Container_1C",
        "fraction": "0",
        "normal": "<0.862338, -0.438635, 0.252928>",
        "oldProbe": "<4511.504395, 340.071808, 10236.768555>",
        "newProbe": "<4511.504395, 340.071777, 10236.758789>",
        "radius": "0.23",
        "floorFound": "true",
        "floorObject": "",
        "floorNormal": "<-0.018661, 0.999818, -0.003999>",
        "newFootGap": "0.669983",
        "probeYDelta": "-3.05176e-05"
      }
    ],
    "conclusion": "All12 terminal rejects are container contact at fixed torso/root probes0..2 or hips3, not foot terrain exit. Terrain-downhill hypothesis contradicted for these witnesses.",
    "limitations": [
      "Nearby recorded frozen poses only; not proof of all possible curved/combined translation-rotation routes.",
      "Raycast floorNormal valid only when floorFound true; otherwise ignore reused output.",
      "No new runtime physical gate changes or visible/performance acceptance."
    ]
  }
}

Own handoff candidate function
override void CompleteEgress(vector support)
    {
        // Ground arrival radius and a root floor hit do not establish that
        // the full pose is outside the container or its navigation inflation.
        // Preserve the existing roof handoff and opt-in wide recovery path.
        if(!ECWideRecovery && Area=="ground" && ECGroundPolicy())
        {
            ECGroundChecks++;
            vector groundSupport;float groundLift;ref array<vector> groundContacts={};string groundRejection;
            if(!LandingSpace(Body.Agent.Position,groundSupport,false))groundRejection="LANDING:"+LandingDiagnostic;
            else if(Fixture().Area(groundSupport)!=Area)groundRejection="AREA";
            else if(!ECEndpointClear(Body.Agent.Position))groundRejection="ENDPOINT_POSE";
            else if(!DGBGroundRoute.SegmentClear(Body.Agent.Position,Body.Agent.Position,Body.Agent.ObstacleMin,Body.Agent.ObstacleMax))groundRejection="PLANNING_INFLATION";
            else if(!Body.SolveContacts(Body.Agent.Position,Body.Agent.Heading,groundLift,groundContacts))groundRejection="CONTACT:"+Body.Reason;
            if(groundRejection!="")
            {
                ECGroundRejected++;ECGroundPending=true;ECGroundLog("REJECTED:"+groundRejection);
                // Keep an existing bounded lease and its distance accounting.
                // The helper, not an unsuccessful base arrival, owns recovery.
                Body.Reason="EGRESS_GROUND_HANDOFF_BLOCKED";return;
            }
            support=groundSupport;Body.Lift=groundLift;Body.Contacts.Copy(groundContacts);ECGroundLog("VALIDATED");
        }
        if(ECWideRecovery)
        {
            // A short lease expiring or the authored egress point being near
            // does not establish clearance of the alternative recovery route.
            vector verified;float lift;ref array<vector> contacts={};
            string rejection;
            if(!LandingSpace(Body.Agent.Position,verified,false))rejection="LANDING:"+LandingDiagnostic;
            else if(Fixture().Area(verified)!=Area)rejection="AREA";
            else if(!ECEndpointClear(Body.Agent.Position))rejection="ENDPOINT_POSE";
            else if(!DGBGroundRoute.SegmentClear(Body.Agent.Position,Body.Agent.Position,Body.Agent.ObstacleMin,Body.Agent.ObstacleMax))rejection="PLANNING_INFLATION";
            else if(!Body.SolveContacts(Body.Agent.Position,Body.Agent.Heading,lift,contacts))rejection="CONTACT:"+Body.Reason;
            if(rejection!="")
            {
                ECWideLog("REJECTED:"+rejection);
                if(ECActive)ECLog("WIDE_HANDOFF_REJECTED");
                Body.Reason="EGRESS_WIDE_HANDOFF_BLOCKED";
                ECActive=false;ECNextPlan=Now();return;
            }
            support=verified;Body.Lift=lift;Body.Contacts.Copy(contacts);
            ECWideLog("VALIDATED");
        }
        super.CompleteEgress(support);
    }

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 진단은 시각·성능 해결을 뜻하지 않는다.
