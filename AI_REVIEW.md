# DayZ NPC AI — 외부 세션 검토 자료

Snapshot: snapshot-20261027T210240-landing-corridor-20261028

착지 차단과 첫 걸음 분리: B8FB ON/OFF 및 컨테이너 정적 재생

작성: 2026-10-28T06:02:06.263048+09:00

1. 작업 목표

고지대 미도달 6·32번의 착지 지연을 군집 점유와 정적 지형/첫 걸음 검사로 구분한다. 같은 B8FB 수정본에서 재접근 플래그만 OFF한 64명 대조를 실행한다. 정상 낙하·복귀와 기존 이동 검사는 보존한다.

2. 실제 수행

- 원래 ON 64명 runtime-20261028-053554와 같은 빌드·미션·그래픽·300초·8부품/개체·상세 계측을 유지하고 runtime-20261028-055226에서 admission reapproach만 OFF했다. 두 실행 정상 종료. OFF 화면의 군집 존재를 확인했으며 새 동영상은 없다.
- ON 기록에서 6·32번의 HIGH_HOLD LANDING_SPACE_BLOCKED 첫/중간/마지막 표본 6개를 선택했다. 같은 컨테이너 엔진 씬에 군집 없이 LandingSpace·5개 위치 변형을 정적으로 재생했다.
- 추가 세부 재생: 150개 바닥 레이 기록, 2위치×33방향×3몸통 높이=198개 3cm 구형 sweep. 8방향과 남쪽 기준 -60~60도 5도 간격을 비교했다. 새 이동 동작은 실행하지 않았다.
- 첫 확장 미션은 함수 명령 수 초과로 컴파일 실패했다. 실패본·로그를 보존하고 공통 함수로 분리한 별도 compact 미션의 엔진 컴파일/실행을 통과했다. PBO는 변경하지 않았다.

3. 측정 결과

|64명 같은 B8FB|ON 053554|OFF 055226|
|---|---:|---:|
|최종 도착|64/64|64/64|
|HIGH_HOLD 미도달|2 (6·32)|5 (3·22·24·41·55)|
|낙하 후 FOLLOW 복귀|76|86|
|등반 종료 검증 실패|0|0|
|출구 저널 계약 위반|0|0|
|출구 DONE / EXPIRED / 다른 상태 인계|10 / 0 / 0|16 / 2 / 6|
|미션 dt p50/p95/p99|30/72/95ms|28/57/79ms|

OFF 물리 전이 977개가 같은 호출 계측과 연결됐다. EXPIRED는 bounded lease의 명시적 종료이며 계약 위반 0과 구분한다. 중간 목표 달성이나 물리적 정체 해소 자체를 입증하지 않는다. 전경 로그 비율 두 실행 100%. 조건당 1회이고 상세 계측·이동 결과가 다르므로 p95 차이 15ms를 순수 재접근 비용이나 최적화 효과로 보지 않는다.

정적 재생: 원위치 6/6 FOOT_X_SUPPORT 거부. 동쪽 발 6/6 정상 바닥. 서쪽 발 6/6은 컨테이너 옆면을 fraction=0에서 맞아 바닥 거부(normalY 약0.014~0.019, 높이차 약0.450m). 이 검사를 완화하지 않았다. 같은 원위치에서 0.3m 남쪽 변형은 4/6 CLEAR였지만 이는 순간 위치 변형 질의이며 실행 가능한 이동을 뜻하지 않는다.

몸통-only sweep: 32번 64.089초 위치는 기존 8방향에서 세 높이를 모두 통과하는 방향 0개, 추가 남쪽 기준 5·10·15·20·25·30·35도 7개였다. 6번 61.23초 위치는 추가 각도에서도 0개다. 후자는 모든 초기 접촉 hit를 막힌 것으로 집계한 결과이며 기존 비침투/초기접촉 이탈 허용을 포함한 전체 PressureEscapeStepClear 결과가 아니다.

4. 원인 분석

확인: 선정 착지 차단은 군집이 없어도 정적 컨테이너 옆면과 바닥 발 지지 검사로 재현된다. 현재 위치의 착지 조건 거부와 안전한 목표로 향하는 첫 물리 이동 거부는 별개다. 원시 escape 계획에서 남쪽 목표는 선택됐지만 첫 걸음이 NEW_WORLD_CONTACT로 거부됐다.

확인: 적어도 한 위치의 고정 8방향 몸통 검사에는 빈 방향이 없지만 더 세밀한 각도에는 있다. 목표의 LandingSpace 성공만으로 중간 경로가 통과한다고 볼 수 없다.

추정: 접촉 법선/목표 방향으로 제한된 추가 후보를 만들고 기존 전체 sweep·비침투·군집·발판 검사를 유지하면 일부 복귀 정체를 해소할 수 있다. 32번의 몸통 통과는 관절·군집·지면까지 가능한 경로가 아니므로 후보 설계의 근거로만 사용한다. 6번은 초기접촉 이탈과 실제 관절/몸통 sweep의 별도 재현이 더 필요하다.

미입증: ON이 미도달 5→2를 인과적으로 줄였다는 해석, OFF의 낮은 p95가 기능 제거의 순수 비용이라는 해석은 단회 대조로 입증하지 못한다. 낙하를 제거하거나 벽 충돌을 무시하는 수정은 하지 않았다.

5. 검증 수준

- 진단 성공: 6개 정적 발 지지 거부 재현, 실패 발 레이 원인, 32번의 고정 방향 표본 누락.
- 엔진 진단 통과: compact 미션의 30착지·150레이·198몸통 sweep 및 기존 바인딩 smoke. 실제 NPC 회귀 통과를 뜻하지 않는다.
- 64명 OFF: 한 실행 최종 도착/등반 종료·저널 계약은 통과, 고지대 기준 미달. ON/OFF 반복 검증 미완료.
- 게임 로직 수정·애니메이션 시각 개선·성능 수용: 미실행/미수용. 20ms p95 목표 미달. 원래 골은 진행 중이다.

6. 변경 사항

추가: stage_horde_landing_replay.py, stage_horde_escape_corridor_replay.py, checkpoint_horde_landing_corridor_20261028.py와 별도 validation 미션/파생 분석. compile_days_gone_native_runtime_probe.ps1에 지정된 landing 진단 미션 이름과 완료/실패 표식 검사만 추가했다. 공개 스테이징 도구는 다른 완료 묶음도 같은 B안 구조에 넣을 수 있도록 매개변수화했다.

게임 소스·정상 이동 파이프라인·PBO·기존 raw/report는 변경하지 않았다. B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185 유지. 기존 서명 검증 통과, 새 서명/재빌드 없음. 운영 서버 변경 없음.

7. 원시 증거

runtime-20261028-053554(ON), runtime-20261028-055226(OFF), 각 native-ground/state-contract/traversal-events/escape-plans 및 분석 JSON. 최초 정적 profile compile-8000a85c453942dba6421b9e81226c55, 실패 확장 compile-15d580d4c42a4a328e0566e573e1a19b, 성공 compact compile-6da3994c2a0049a48806d84d09c4608c. recorded-landing.tsv·recorded-floor-detail.tsv·recorded-core-corridor.tsv와 staged/engine-compile JSON을 보존했다. 아래 로컬 목록에서 경로·SHA-256을 검증한다. 공개 패킷은 선정 함수/결과/레이 표본만 제공하며 내부 경로와 프로세스 정보는 제외한다.

8. 잔여 문제

1. 추가 방향은 접촉 법선 기반의 소수 후보로 제한하고 기존 8방향이 실패할 때만 평가하는 후보를 설계한다. 먼저 기록 입력에서 원래 초기접촉 처리/본 sweep까지 비교해야 한다. 통과 시 opt-in 기능 후보로 빌드·서명·회귀 검증한다. 단순 각도 전수 탐색을 매 틱 추가하지 않는다.
2. 6번의 실제 world 시작 접촉과 법선·본 위치를 읽기 계측/기록 재생으로 좁힌다. 전체 경로가 물리적으로 없는 경우와 후보 방향 누락을 구분한다.
3. ON/OFF 순서 교차 반복으로 군집 변동을 측정한다. 기능 기준 64/64 유효 종료와 중간 고지대 도달을 함께 유지한다.
4. CPU/표시 시간 불일치와 buffer 연속성은 별도 진행한다. 이번 스크린샷·콜백·정적 재생으로 애니메이션 끊김을 해결했다고 보고하지 않는다. 성능 4조건 대조와 직접 계측 불가 범위도 별도 유지한다.

원본 보고서 ID: 2026-10-28_060206_263048+0900_npc-landing-corridor.md
원본 SHA-256: c482a3e29f0e546c2ca35013094aa80278627c613eae350dacb9a31e4a68cd9f
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

===== 선정 원시 근거 (전체 원본 아님) =====
ON/OFF 분석 JSON 선택 필드:
{
  "on": {
    "runId": "runtime-20261028-053554",
    "count": 64,
    "durationSeconds": 300,
    "buildId": "B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185",
    "terminalArrivals": 64,
    "unresolvedTerminalAgents": [],
    "missingHighFollow": [
      6,
      32
    ],
    "highHoldWindow": [
      45.083,
      104.927
    ],
    "fallsLandedAndResumed": 76,
    "climbEndFailures": 0,
    "physicalTransitionsMatched": true,
    "physicalTransitions": 916,
    "admissionOutcomes": {
      "ARRIVED": 15,
      "INTENT_CHANGED": 1
    },
    "admissionViolations": [],
    "egressOutcomes": {
      "DONE": 10
    },
    "egressViolations": [],
    "foregroundMissionFrameMs": {
      "samples": 8246,
      "p50": 30.0,
      "p95": 72.0,
      "p99": 95.0
    },
    "foregroundFraction": 1.0,
    "motorCallbackHz": {
      "minimum": 15.271427413128773,
      "median": 15.271427413128773,
      "maximum": 15.271427413128773
    },
    "visibleCallbackHz": {
      "minimum": 15.271427413128773,
      "median": 15.271427413128773,
      "maximum": 15.271427413128773
    },
    "repetitions": 1
  },
  "off": {
    "runId": "runtime-20261028-055226",
    "count": 64,
    "durationSeconds": 300,
    "buildId": "B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185",
    "admissionEnabled": false,
    "terminalArrivals": 64,
    "missingHighFollow": [
      3,
      22,
      24,
      41,
      55
    ],
    "highHoldWindow": [
      45.002,
      104.898
    ],
    "fallsLandedAndResumed": 86,
    "climbEndFailures": 0,
    "admissionOutcomes": {},
    "admissionViolations": [],
    "egressOutcomes": {
      "DONE": 16,
      "EXPIRED": 2,
      "PHASE_HANDOFF:FOLLOW": 6
    },
    "egressViolations": [],
    "foregroundMissionFrameMs": {
      "samples": 9367,
      "p50": 28.0,
      "p95": 57.0,
      "p99": 79.0
    },
    "foregroundFraction": 1.0,
    "repetitions": 1
  }
}
정적 원위치 6개만 선택 (30개 중 offset0). 추가30cm 변형은 보고서 집계만 제공:
[
  {
    "stamp": "51.644",
    "id": "6",
    "root": "<4511.480957, 339.398163, 10239.166016>",
    "offset": "<0.000000, 0.000000, 0.000000>",
    "near": "<4511.480957, 339.398163, 10239.166016>",
    "floorValid": "true",
    "floor": "<4511.480957, 339.178772, 10239.166016>",
    "valid": "false",
    "diagnostic": "FOOT_X_SUPPORT",
    "support": "<4511.480957, 339.178772, 10239.166016>",
    "fixture035": "false",
    "fixture090": "false",
    "fixture150": "false",
    "world035": "false:none",
    "world090": "true:StaticObj_Container_1C",
    "world150": "false:none"
  },
  {
    "stamp": "63.365",
    "id": "6",
    "root": "<4511.492676, 339.278229, 10236.863281>",
    "offset": "<0.000000, 0.000000, 0.000000>",
    "near": "<4511.492676, 339.278229, 10236.863281>",
    "floorValid": "true",
    "floor": "<4511.492676, 339.171997, 10236.863281>",
    "valid": "false",
    "diagnostic": "FOOT_X_SUPPORT",
    "support": "<4511.492676, 339.171997, 10236.863281>",
    "fixture035": "false",
    "fixture090": "false",
    "fixture150": "false",
    "world035": "false:none",
    "world090": "false:none",
    "world150": "false:none"
  },
  {
    "stamp": "75",
    "id": "6",
    "root": "<4511.494629, 339.278229, 10236.852539>",
    "offset": "<0.000000, 0.000000, 0.000000>",
    "near": "<4511.494629, 339.278229, 10236.852539>",
    "floorValid": "true",
    "floor": "<4511.494629, 339.171967, 10236.852539>",
    "valid": "false",
    "diagnostic": "FOOT_X_SUPPORT",
    "support": "<4511.494629, 339.171967, 10236.852539>",
    "fixture035": "false",
    "fixture090": "false",
    "fixture150": "false",
    "world035": "false:none",
    "world090": "false:none",
    "world150": "false:none"
  },
  {
    "stamp": "52.151",
    "id": "32",
    "root": "<4511.481934, 339.290375, 10238.037109>",
    "offset": "<0.000000, 0.000000, 0.000000>",
    "near": "<4511.481934, 339.290375, 10238.037109>",
    "floorValid": "true",
    "floor": "<4511.481934, 339.175751, 10238.037109>",
    "valid": "false",
    "diagnostic": "FOOT_X_SUPPORT",
    "support": "<4511.481934, 339.175751, 10238.037109>",
    "fixture035": "false",
    "fixture090": "false",
    "fixture150": "false",
    "world035": "false:none",
    "world090": "false:none",
    "world150": "false:none"
  },
  {
    "stamp": "55.845",
    "id": "32",
    "root": "<4511.482422, 339.275391, 10236.887695>",
    "offset": "<0.000000, 0.000000, 0.000000>",
    "near": "<4511.482422, 339.275391, 10236.887695>",
    "floorValid": "true",
    "floor": "<4511.482422, 339.171906, 10236.887695>",
    "valid": "false",
    "diagnostic": "FOOT_X_SUPPORT",
    "support": "<4511.482422, 339.171906, 10236.887695>",
    "fixture035": "false",
    "fixture090": "false",
    "fixture150": "false",
    "world035": "false:none",
    "world090": "true:StaticObj_Container_1C",
    "world150": "false:none"
  },
  {
    "stamp": "59.606",
    "id": "32",
    "root": "<4511.497070, 339.275391, 10236.858398>",
    "offset": "<0.000000, 0.000000, 0.000000>",
    "near": "<4511.497070, 339.275391, 10236.858398>",
    "floorValid": "true",
    "floor": "<4511.497070, 339.172058, 10236.858398>",
    "valid": "false",
    "diagnostic": "FOOT_X_SUPPORT",
    "support": "<4511.497070, 339.172058, 10236.858398>",
    "fixture035": "false",
    "fixture090": "false",
    "fixture150": "false",
    "world035": "false:none",
    "world090": "false:none",
    "world150": "false:none"
  }
]
서쪽 발6개만 선택 (150개 레이 중 원위치/probe2):
[
  {
    "stamp": "51.644",
    "id": "6",
    "variant": "<0.000000, 0.000000, 0.000000>",
    "probe": "2",
    "near": "<4511.250977, 339.178772, 10239.166016>",
    "rayHit": "true",
    "hitType": "StaticObj_Container_1C",
    "point": "<4511.250977, 339.628784, 10239.166016>",
    "normal": "<0.999826, 0.018665, 0.000030>",
    "fraction": "0",
    "heightDifference": "0.450012",
    "floorAccepted": "false"
  },
  {
    "stamp": "63.365",
    "id": "6",
    "variant": "<0.000000, 0.000000, 0.000000>",
    "probe": "2",
    "near": "<4511.262695, 339.171997, 10236.863281>",
    "rayHit": "true",
    "hitType": "StaticObj_Container_1C",
    "point": "<4511.262695, 339.622009, 10236.863281>",
    "normal": "<0.938230, 0.016593, -0.345615>",
    "fraction": "0",
    "heightDifference": "0.450012",
    "floorAccepted": "false"
  },
  {
    "stamp": "75",
    "id": "6",
    "variant": "<0.000000, 0.000000, 0.000000>",
    "probe": "2",
    "near": "<4511.264648, 339.171967, 10236.852539>",
    "rayHit": "true",
    "hitType": "StaticObj_Container_1C",
    "point": "<4511.264648, 339.621979, 10236.852539>",
    "normal": "<0.823402, 0.013858, -0.567289>",
    "fraction": "0",
    "heightDifference": "0.450012",
    "floorAccepted": "false"
  },
  {
    "stamp": "52.151",
    "id": "32",
    "variant": "<0.000000, 0.000000, 0.000000>",
    "probe": "2",
    "near": "<4511.251953, 339.175751, 10238.037109>",
    "rayHit": "true",
    "hitType": "StaticObj_Container_1C",
    "point": "<4511.251953, 339.625763, 10238.037109>",
    "normal": "<0.999826, 0.018655, -0.000006>",
    "fraction": "0",
    "heightDifference": "0.450012",
    "floorAccepted": "false"
  },
  {
    "stamp": "55.845",
    "id": "32",
    "variant": "<0.000000, 0.000000, 0.000000>",
    "probe": "2",
    "near": "<4511.252441, 339.171906, 10236.887695>",
    "rayHit": "true",
    "hitType": "StaticObj_Container_1C",
    "point": "<4511.252441, 339.621918, 10236.887695>",
    "normal": "<0.999826, 0.018663, 0.000013>",
    "fraction": "0",
    "heightDifference": "0.450012",
    "floorAccepted": "false"
  },
  {
    "stamp": "59.606",
    "id": "32",
    "variant": "<0.000000, 0.000000, 0.000000>",
    "probe": "2",
    "near": "<4511.267090, 339.172058, 10236.858398>",
    "rayHit": "true",
    "hitType": "StaticObj_Container_1C",
    "point": "<4511.267090, 339.622070, 10236.858398>",
    "normal": "<0.906577, 0.015795, -0.421745>",
    "fraction": "0",
    "heightDifference": "0.450012",
    "floorAccepted": "false"
  }
]
몸통3높이 모두 unfiltered native sweep hit false인 방향만 선택 (198개 중 그룹 통과). 전체 이동 승인 아님:
[
  {
    "id": "32",
    "family": "south-angle",
    "angle": 5.0
  },
  {
    "id": "32",
    "family": "south-angle",
    "angle": 10.0
  },
  {
    "id": "32",
    "family": "south-angle",
    "angle": 15.0
  },
  {
    "id": "32",
    "family": "south-angle",
    "angle": 20.0
  },
  {
    "id": "32",
    "family": "south-angle",
    "angle": 25.0
  },
  {
    "id": "32",
    "family": "south-angle",
    "angle": 30.0
  },
  {
    "id": "32",
    "family": "south-angle",
    "angle": 35.0
  }
]

SOURCE DGBNativeTraversal.c; SHA256 71d4a73d4cf89334320236bdb1365f416436cb3cdb15ab959613d4d2916e9872; LandingSpace 전체; lines 180-214
180:     bool LandingSpace(vector nearPoint,out vector supported,bool requireEntitySpace=true)
181:     {
182:         LandingChecks++;LandingDiagnostic="ROOT_SUPPORT";
183:         if(!DGBGroundAgent.Floor(Body.Leader,nearPoint,supported))return false;
184:         vector left,right;
185:         LandingDiagnostic="FOOT_X_SUPPORT";
186:         if(!DGBGroundAgent.Floor(Body.Leader,supported+"0.23 0 0",left))return false;
187:         if(!DGBGroundAgent.Floor(Body.Leader,supported+"-0.23 0 0",right))return false;
188:         if(Math.AbsFloat(left[1] - right[1])>0.22)return false;
189:         LandingDiagnostic="FOOT_Z_SUPPORT";
190:         if(!DGBGroundAgent.Floor(Body.Leader,supported+"0 0 0.23",left))return false;
191:         if(!DGBGroundAgent.Floor(Body.Leader,supported+"0 0 -0.23",right))return false;
192:         if(Math.AbsFloat(left[1] - right[1])>0.22)return false;
193:         if(requireEntitySpace && !Body.Agent.ManagedPeerContacts)
194:             foreach(DGBGroundAgent peer:Body.Director.Agents)
195:                 if(peer!=Body.Agent && peer.State!="DEAD" && Math.AbsFloat(peer.Position[1] - supported[1])<1 && DGBGroundAgent.FlatDistance(peer.Position,supported)<0.85)
196:                 {LandingDiagnostic="PEER:"+Body.Director.Agents.Find(peer);return false;}
197:         if(requireEntitySpace && Body.Director.CrowdDynamics)
198:             foreach(Man physicalPlayer:Body.Director.PerceivedPlayers)
199:                 if(physicalPlayer && physicalPlayer.IsAlive() && Math.AbsFloat(physicalPlayer.GetPosition()[1] - supported[1])<1.4 && DGBGroundAgent.FlatDistance(physicalPlayer.GetPosition(),supported)<0.7)
200:                 {LandingDiagnostic="PLAYER";return false;}
201:         array<float> heights={0.35,0.9,1.5};
202:         foreach(float height:heights)
203:         {
204:             Object hit;vector point,normal;float fraction;
205:             vector center=supported+Vector(0,height,0);
206:             if(Fixture().Occupied(center,0.23)){LandingDiagnostic="FIXTURE_VOLUME";return false;}
207:             if(DayZPhysics.SphereCastBullet(center,center+"0 0.001 0",0.23,DGBGroundAgent.Walls(),Body.Leader,hit,point,normal,fraction))
208:             {LandingDiagnostic="WORLD";if(hit){ LandingDiagnostic="WORLD:"+hit.GetType(); }return false;}
209:         }
210:         LandingDiagnostic="CLEAR";
211:         return true;
212:     }
213:     void Walk(vector goal,float arrival,bool platform,float dt,vector delta,bool available)
214:     {

SOURCE DGBNativeTraversal.c; SHA256 71d4a73d4cf89334320236bdb1365f416436cb3cdb15ab959613d4d2916e9872; 계획 후보 및 실패 기록 일부; lines 880-927
880:                 // one direction use exactly the same first physical step.
881:                 if(blockedFirstSteps[directionIndex]){EscapeDuplicateDirectionsAvoided++;continue;}
882:                 vector direction=directions[directionIndex];
883:                 vector target=Body.Agent.Position+direction*distance;vector supported;
884:                 if(Phase=="FALL")target=DGBTraversalFixture.Ground(target);
885:                 else
886:                 {
887:                     vector candidateSupport;
888:                     if(!DGBGroundAgent.Floor(Body.Leader,target,candidateSupport)){if(tracePlan)outcomes[directionIndex]="TARGET_NO_FLOOR\t"+target;continue;}
889:                     if(Fixture().Area(candidateSupport)!=Fixture().Area(Body.Agent.Position)){if(tracePlan)outcomes[directionIndex]="TARGET_AREA_CHANGED\t"+target;continue;}
890:                     if(Math.AbsFloat(candidateSupport[1] - Body.Agent.Position[1])>0.08){if(tracePlan)outcomes[directionIndex]="TARGET_HEIGHT_CHANGED\t"+target;continue;}
891:                     target=candidateSupport;
892:                 }
893:                 if(!LandingSpace(target,supported)){if(tracePlan)outcomes[directionIndex]="TARGET_"+LandingDiagnostic+"\t"+target;continue;}
894:                 float candidateLift;ref array<vector> points={};
895:                 if(!PressureEscapeStepClear(Body.Agent.Position+direction*Math.Min(0.04,dt*0.6),candidateLift,points))
896:                 {
897:                     PressureEscapeLastStepReject=PressureEscapeDiagnostic;blockedFirstSteps[directionIndex]=true;
898:                     if(tracePlan)outcomes[directionIndex]=EscapeDirectionFailure(supported);
899:                     continue;
900:                 }
901:                 EscapeNoExitAt=-1;
902:                 PressureEscapeGoal=supported;PressureEscapeValid=true;LandingDiagnostic=savedLanding;return true;
903:             }
904:         }
905:         if(EscapeNoExitAt<0)EscapeNoExitAt=Now();
906:         if(tracePlan)TraceEscapePlan(outcomes,dt);
907:         LandingDiagnostic=savedLanding;PressureEscapeDiagnostic="NO_VALID_LOCAL_EXIT";return false;
908:     }
909:     string EscapeDirectionFailure(vector target)
910:     {
911:         // Fields are valid for this failed query only, not the previous probe.
912:         string row=PressureEscapeDiagnostic+"\t"+target;
913:         if(PressureEscapeDiagnostic!="DEEPER_FIXTURE" && PressureEscapeDiagnostic!="NEW_WORLD_CONTACT")
914:             return row+"\t-1\t\t0 0 0\t-1\t0\t0\t-1\t0 0 0\t0 0 0\t0 0 0";
915:         row+="\t"+PressureEscapeRejectProbe+"\t"+PressureEscapeRejectFixture+"\t"+PressureEscapeRejectFixturePosition+"\t"+PressureEscapeRejectFixtureIndex;
916:         row+="\t"+PressureEscapeRejectBefore+"\t"+PressureEscapeRejectAfter;
917:         row+="\t"+PressureEscapeRejectFraction+"\t"+PressureEscapeRejectNormal+"\t"+PressureEscapeRejectStart+"\t"+PressureEscapeRejectEnd;
918:         return row;
919:     }
920:     void TraceEscapePlan(array<string> outcomes,float dt)
921:     {
922:         NextEscapePlanTrace=Now()+1;EscapePlanTraceBatches++;
923:         FileHandle log=OpenFile("$profile:native-escape-plans.tsv",FileMode.APPEND);if(!log)return;
924:         for(int direction=0;direction<outcomes.Count();direction++)
925:         {
926:             string detail=outcomes[direction];
927:             if(detail.IndexOf("TARGET_")==0)detail+="\t-1\t\t0 0 0\t-1\t0\t0\t-1\t0 0 0\t0 0 0\t0 0 0";

SOURCE DGBNativeTraversal.c; SHA256 71d4a73d4cf89334320236bdb1365f416436cb3cdb15ab959613d4d2916e9872; Fall 착지 시도 일부; lines 1230-1251
1230:     void Fall(float dt)
1231:     {
1232:         if(!Body.FrameFallPhysicsActive())
1233:         {
1234:             if(Recovering && Body.Request!=8)return;
1235:             if(!Recovering && Body.Request!=6 && Body.Request!=8)return;
1236:         }
1237:         bool escaped=EscapeStep(dt);
1238:         FallVelocity=Math.Max(-15,FallVelocity - 9.81*dt);
1239:         vector next=Body.Agent.Position+Vector(0,FallVelocity*dt,0);
1240:         if(PressureFallActive)next+=PressureFallVelocity*dt;
1241:         Object hit;vector point,normal;float fraction;bool landed=false;
1242:         if(DayZPhysics.RayCastBullet(Body.Agent.Position+"0 0.12 0",next+"0 -0.1 0",DGBGroundAgent.Floors(),Body.Leader,hit,point,normal,fraction))
1243:         {
1244:             if(normal[1]>=0.65 && point[1]<=Body.Agent.Position[1]+0.05 && point[1]>=next[1] - 0.1)
1245:             {
1246:                 vector support;if(LandingSpace(point,support,!Body.Agent.ManagedPeerContacts)){next=support;landed=true;}
1247:                 else{if(!escaped){if(PressureFallActive){if(!PressureSlide(dt))AirEscapeTurn(dt);}else EscapeStep(dt);}Body.Reason="LANDING_SPACE_BLOCKED";return;}
1248:             }
1249:         }
1250:         if(!Accept(next,dt)){if(!escaped){if(PressureFallActive){if(!PressureSlide(dt))AirEscapeTurn(dt);}else EscapeStep(dt);}Body.Reason="FALL_"+Body.Reason;return;}
1251:         Body.Reason="NATIVE_FALL";

입력 기록/함수 발췌만으로 본/물리 전체 연속성 또는 영상 부드러움을 판정하지 말 것. 실패 확장 코드는 function instruction cap으로 실행되지 않았다. 원본 해시와 선정 기준은 보고서에 있다.

이전 애니메이션 진단: [선정 근거](downloads/animation-review.txt). 이번 기능 시험은 시각·성능 해결을 뜻하지 않는다.
