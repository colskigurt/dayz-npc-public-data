> 최신 진단(2026-10-31): 12번 낙하 정체의 관절 충돌·예산 소진을 재현했고,착지 복구의1.5m 예산이 다음 낙하에 남는 상태값 누출을 확인했다. 임무전용복원후보는반복상태검사통과,새PBO·8/64·화면·성능은미검증이다. [근거와미수용범위](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/actor12-fall-budget-20261031.md). 아래는과거보존기록이다.

> 최신 실제 비교(2026-10-31): 같은 E57B 후보의8/64 OFF·ON 각300초 결과를 기록했다. 실패·후보 호출·정상 낙하 복귀를 구분하며,반복 회귀·화면 품질·성능 수용은 미완료다. [실행별 결과와 한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/endpoint-bounded-runtime-20261031.md). 아래는 과거 보존 기록이다.

> 새 후보 패키지(2026-10-31): E57B 끝점 선택기 벡터 한도만 수정. 빌드·서명·검증 및 동결 대조216행 통과. 기본 작업본 보존,새8→64·시각·성능 수용은 미실행이다. [후보 범위와 검증](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/endpoint-bounded-package-20261031.md). 아래는 이전 실제 시험/진단 기록이다.

> 최신 진단(2026-10-31): 60번의 −Z 표면 법선 거부를 17조건×2회 재현. 같은 호출의 raw4/이전접촉4로 실제 Accept 출력도 일치했다. 기존 선택기의 수치 한도 후보는 6dt×2회 통과했지만 게임 PBO·새8→64에는 미반영이다. [원인·후보·한계](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/actor60-footprint-accept-20261031.md). 아래 실제 기능 수치는 기존 시험 기록이다.

> 최신 실제 시험(2026-10-31): 고지대 목표 유지 조건에서8/8·64/64고지대 및 최종 도착.64개체 종료 검사1건,낙하 복귀73건,p95 76ms.반복 기능·화면 품질·성능 미수용. [실행 조건과 실패 증거](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/held-high-eight-sixtyfour-20261031.md). 아래 과거 수치는 원본 이력이다.

> 시험 조건 정정(2026-10-31): 기존64개체 HIGH_HOLD의 실제 플레이어 high 유지 약29.5초. 실제 high 유지 중 high+FOLLOW60/64이며, 최종64/64 도착과 별도다. 신규 런타임·수정·시각/성능 수용 아님. [원시 위치 기반 재분석](https://raw.githubusercontent.com/colskigurt/dayz-npc-public-data/main/reports/operations/high-stimulus-validity-20261031.md).

> 새 애니메이션 진단(2026-10-31): idle·walk 1논리개체/8부품 고정시험에서 native 시간 진행률0.640/0.668, CPU/화면 대응 확인. 새 수정·8/64회귀·품질/성능 수용은 아님. [상세 결과](operations/idle-walk-visible-20261031.md). 아래는 이전 기능 실험 기록이다.

> 2026-10-31 코드 분석: GPUSkinning과 crowd-simulation을 고정 커밋으로 비교했다. 런타임 변경·신규 실험 없음. [적용 범위와 한계](operations/external-horde-source-review-20261031.md). 아래 실험 시각·측정값은 종전 그대로다.

# 복구 후보 실제 8→64 시험: 최종 도착과 미해결 고지대 도달 구분

> 운영 변경: Steam 최소 작업본을 기존 경로에 세팅했다. 대용량 삭제는 미완료다. 아래 실험 시각·수치는 변경하지 않았다. [최신 작업 상태](operations-latest.md)

중간 방향 복구 후보: 패키지 계약과 실제 8→64 회귀

작성: 2026-10-30T09:40:34.176533+09:00

1. 작업 목표

동결 입력에서 검증한 중간 방향 탐색을 실제 서명 패키지와8→64개체 씬에서 검사한다. 정상 낙하·복구와 벽/지지 규칙을 유지하고, 실제 기능·시각 품질·성능을 구분한다.

2. 실제 수행

기존13개 스크립트와 원래 live mission은 바이트 동일하게 유지했다. 기본OFF의FOLLOW 복구 모듈1개를 별도 아카이브로 추가했다. 기존8방향 실패,PoseClearanceActive,활성 연결 없음,진입 정렬 아님을 모두 만족할 때 중간8방향만 검사한다. 원래 PressureSlide가 실제 이동을 수행한다.

패키지 강제 OFF/ON/기본 flag 없음, 7개 상태/기존 성공 경로 대조, 벽 관통·지지 불량 음성 대조 16행을 두 번 검사했다. 기본 없음과 OFF가 행 단위 동일하고 제외 상태에서 추가 계획 0이었다. 같은 패키지 ON은 고정 입력에서 0.507013m 탈출했다. 실제 flag 파일이 있는 상태의 cold read도 추가 검사했고 강제 ON과 행 단위 동일했다. 이어 실제 클라이언트의 기존 activeReference, 300초, 동일 계측 조건을 사용했다.

3. 측정 결과

| 개체 | 설정 | 최종 도착 | HIGH_HOLD 미도달 | 낙하 후 재개 | 등반 종료 실패 | mission p50/p95/p99 ms |
|---|---|---|---|---|---|---|
| 8 | on | 8/8 | [] | 9 | 0 | 20.0/33.0/34.0 |
| 64 | on | 64/64 | [17, 33, 40] | 77 | 0 | 26.0/64.0/86.0 |

패키지 상태 대조 48행과 동결 방향 검사 288행을 세 실행에 보존했다(기본 OFF 두 반복 및 ON 파일 읽기 1회). 실제 trace의 선정 수와 실행 상태는 아래 JSON에 기록한다. p95는 진단 로그가 포함된 foreground mission dt이며 엔진 CPU/GPU 기여도나 표시 프레임 측정이 아니다. 이번 실험으로 애니메이션 개선을 판정하지 않는다.

이번 실제 ON 시험은 8/8·64/64 최종 도착이지만 64개체 HIGH_HOLD에서 17·33·40번이 미도달했다. 14번의 EGRESS_STALL 1건은 나중에 도착했으며 등반 종료 실패는 이번에 재현되지 않았다. 두 실제 실행 모두 추가 탐색 trace가 없어 실제 분기 효과는 관측하지 못했다. 이 결과를 중간 방향 수정의 성공으로 귀속하지 않는다. 이전 실패의 수리도 증명하지 못했다.

4. 원인 분석

확인된 실제 상태와 추가 계획의 표본:
json
[
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
]


선정 goal은 계획 출력이며 즉시 위치 커밋이 아니다. 계획 선정과 최종 회복의 원인 관계는 상태/연속 기록을 연결해야 한다. 128행/개체·2048행 전체 제한과 IO 미계측 때문에 trace 없음을 분기 미실행으로 단정하지 않는다. 원래 회전 복구가 재시도 시계를 다시 열 수 있어 0.75초를 엄격한 최소 간격으로 주장하지 않는다.

추정: 좁은 허용 방향에 대한 해상도 확대가 일부 접촉 복구를 돕는다. 실제 전반적 개선과 비용은 이 표본만으로 확정하지 않는다. 이전 12번 미착지·50번 등반 종료 거부는 별도 재현/수정 대상으로 보존한다.

5. 검증 수준

패키지 기본 OFF 계약은 두 반복 검증 완료, 실제 ON 파일 읽기는 1회 진단 성공. 실제 8/64는 위 실행별 결과에 한정한다. 실제 조건을 맞춘 OFF/ON 순서 교차 반복, 시각 시간 불일치 해결, 20ms p95 목표는 미수용이다. 한 실행의 64/64 또는 실패 0만으로 전체 골 완료를 선언하지 않는다.

6. 변경 사항

아카이브PBO 2FB65EB1FD77E334F8A36CF7ED5E4F4F0439C6B8CDD56976ECBDB13C0F70D226는 빌드·서명·검증 및 14개 packed-source 일치를 확인했다. 개인 키 미포함. 작업용 D61D PBO와 운영 서버는 변경하지 않았다. compile/launcher에 명시된 별도 validation과 on/off 설정만 추가했다. 기존 씬의 자동 플레이어 이동·애니메이션 시계·접촉 규칙은 바꾸지 않았다.

7. 원시 증거

실행ID: runtime-20261030-092351, runtime-20261030-093001. 각 launch, mission snapshot, 상태 계약, 고지대 coverage, 착지/진입 분석, 접촉 입력, 추가 탐색 TSV와 패키지 검증을 보존했다. 검토 패킷은 실제 모듈 원문과 선정 계획 표본이며 전체 원시 로그는 아니다. 값이 없는 측정은 0으로 대체하지 않았다.

8. 잔여 문제

실제 미해결 사례가 있으면 해당 단계의 동일 호출 입력을 먼저 분리한다. 회복된 사례도 정상 낙하·착지·추적 재개와 목표 갱신이 이어졌는지 연결한다. 같은 패키지의 OFF/ON 조건을 순서 교차 반복하고, 추가 계획량은 빈도와 시간 범위를 나눠 측정한다. 이후 애니메이션 CPU 포즈/표시 시간 실험과 64개체 비용 기여도 실험을 별도로 진행한다. 새 관통·지지 누락·무제한 탐색·상태 계약 회귀 시 후보 수용을 보류한다.

원본 보고서 ID: 2026-10-30_094034_176533+0900_npc-refinement-runtime.md
원본 SHA-256: f8aaa758e581fa21ae5114533bd1aaeab610fb7800951f161e76a2421daad9aa
이 텍스트는 복사용 파생본입니다. 수치·검증 한계는 원본과 같으며 상세 파일 해시 목록은 원본에 보존되어 있습니다.

선정 함수·대표 기록: [검토 패킷](../downloads/checkpoints/escape-refinement-runtime-review.txt)

[이전 기능 분석](../downloads/admission-report.txt) · [이전 애니메이션 분석](../downloads/animation-report.txt)
