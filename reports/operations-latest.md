# B8FB 기능 재관측: 180초 잔여 개체를 보존하고 동일300초8→64를 대조

작성: 2026-10-31T12:14:40.135827+09:00

## 1. 작업 목표

보존된 B8FB 재접근 전달 수정의 실제8→64기능 회귀를 다시 관측한다. 정상 군집 밀림·낙하·복귀를 미해결 출구 혼잡/등반 종료 실패와 구분한다. 애니메이션 시간 실험과64성능 최적화는 별도 판정한다.

## 2. 실제 수행

당시 B8FB 보관 파일24개 hash와 기존 미션 F8356C4F1E0286DCE7ED83F0137E68061249D48ABEB175942D44A8A048D704D3를 대조했다. 보관된 실행기를 파생해 signed B8FB 경로를 직접 사용했다. 공용 D61D를 되돌리거나 덮어쓰지 않았다. activeReference, StateContract/DriveAttribution/extended egress/first·near first step/endpoint lookahead/terrain contact owner/pursuit path revalidation/admission reapproach,VSyncON·같은 graphics seed를 유지했다.8명180초 후8명300초를 추가하고64명300초를 실행했다.

첫8/180에서 마지막 관측상7/8만ARRIVED여서 통과시키지 않았다.5번은175.095초 gap4.31662m에서179.706초0.334036m까지 이동 중이었고, 마지막 표본과 종료 요청 사이 약0.294초가 남았다. 그 이후 상태를 추정해 통과로 바꾸지 않았다. 고지대8/8·출구/등반 위반0과 최종 미도착을 분리했다.300초 추가는64시험과 기간을 맞추고 마지막 정지 후 시간을 더 관측하기 위한 별도 시험이다. 원래180초 판정을 유지하고 plan-adjustment에 기록했다.

세 실행 모두 정상 종료 후 기존 분석기5개와 별도 terminal-window 분석을 사용했다. entityID/이전·다음상태/이유/recovery generation/위치/같은 호출 contact와 snapshot age/최종 결과를 기존CSV·JSON에 연결했다. 고지대 판정은 실제HIGH_HOLD 표본 경계를 유지했다. 각HIGH_HOLD에서 게임 창만45초 녹화했고 컴퓨터 유즈·4시점 이미지로 장면을 확인했다. 사람용 시각 확인이며 프레임별 부드러움 수용 시험은 아니다.

## 3. 측정 결과

| 실행 | 명/초 | 최종 ARRIVED | HIGH_HOLD 누락ID | 낙하 후 복귀 | 출구 stall 표본 episode | 등반 종료 실패 | 해당 실행 기능 기준 |
|---|---:|---:|---|---:|---:|---:|---|
|runtime-20261031-115150|8/180|7/8|[]|9|0|0|False|
|runtime-20261031-115839|8/300|8/8|[]|9|0|0|True|
|runtime-20261031-120524|64/300|64/64|[5, 8, 11, 18, 22, 31, 39, 45, 49, 50, 53, 56]|71|0|0|False|

- runtime-20261031-115150: mission p50/p95/p99 20.0/33.0/34.0ms; 재접근 {"ARRIVED": 2}, 위반 0; 출구 {}, 위반 0; 전이 112개 same-call 연결 True.
- runtime-20261031-115839: mission p50/p95/p99 20.0/33.0/34.0ms; 재접근 {"ARRIVED": 1}, 위반 0; 출구 {"PHASE_HANDOFF:FOLLOW": 1}, 위반 0; 전이 112개 same-call 연결 True.
- runtime-20261031-120524: mission p50/p95/p99 33.0/69.0/97.0ms; 재접근 {"ARRIVED": 18}, 위반 0; 출구 {"DONE": 16, "PHASE_HANDOFF:FOLLOW": 2, "STEP_REJECTED:DEEPER_FIXTURE": 3}, 위반 0; 전이 815개 same-call 연결 True.

잔여 분리:
- eight: 최종 미도착 [{"id": "5", "position": "<4564.152832, 339.419189, 10282.879883>", "state": "CHASE", "traversalPhase": "FOLLOW", "reason": "", "seenAge": "0", "poseEpisodeAge": "-1", "poseEpisodeStatus": "ESCAPE_MOVED", "pressureEscapeDiagnostic": "NONDEEPENING_CLEAR", "pressureEscapeLastStepReject": "", "pressureEscapeRejectProbe": "-1", "pressureEscapeRejectFixture": "", "pressureEscapeRejectBefore": "0", "pressureEscapeRejectAfter": "0"}]; 고지대 누락 []; 미해결 출구 stall [].
- eight-long: 최종 미도착 []; 고지대 누락 []; 미해결 출구 stall [].
- sixty-four: 최종 미도착 []; 고지대 누락 [5, 8, 11, 18, 22, 31, 39, 45, 49, 50, 53, 56]; 미해결 출구 stall [].

동일300초8→64쌍 전체 통과 여부:False. 전체3시험 모두 통과:False. 마지막 표본은 동기화된 shutdown 엔진 snapshot이 아니며, terminal-window JSON에 개체별 마지막시각·남은시간·FINAL_HOLD 도착시각/표본비율을 보존했다. 반복성을 입증한 최종 수용으로 확대하지 않는다.20ms p95는 목표다.

## 4. 원인 분석

확인: 첫8/180의5번은 terminal이 유효하다는 근거가 부족해 실패로 유지했다. 이동 거리 감소와 이전 도착 기록은 계속 추격하는 상태라는 증거이며, 종료 후 도착했을 것이라는 추정은 수용하지 않았다. 추가300초는 별도 실행이므로 동일5번의 과거 상태를 복원하거나 원인을 확정하지 않는다. sample cadence에 따른 관측 공백과 실제 추격 지연을 구분한다.

낙하 episode는 generation별 착지/추적 복귀를 집계했다. 출구 stall 이후ARRIVED가 관측되면 복구로 표시하되 근본 원인 수정이나 같은 원래 목표 도달의 증명으로 쓰지 않는다. 고지대에 늦게 도착했거나 플레이어가 내려온 뒤 최종 도착했다고 해서HIGH_HOLD 누락을 지우지 않는다. 등반 종료 계약 실패는 회복했더라도 별도로 남긴다.

고지대 누락12명의 진행 분류:5·11·50번은해당구간MOTION시작0회,18·22·49번은낙하/착지복구후새MOTION시작0회,31·39·45·53·56번은해당구간지면→낮은면등반만시작했고,8번은상부등반을시작했으나기한안에마치지못했다.12명모두같은호출의고지대MOTION→SETTLE전이도기한안에는0회여서단순FOLLOW표본누락으로통과시키지않는다.8번은106.968초상부동작을마치고107.838초highFOLLOW,45번은108.548초/109.401초로늦게이어졌다.원래기한104.982초를연장하지않았다.
+
+12명모두고지대구간표본간이동거리가약23.15~31.96m였다.따라서가만히멈춰있었다는설명은지지되지않고,움직이면서정렬·접근·복구를반복한것으로기록된다.이는경로또는접촉의물리근본원인을확정한분류가아니다.진행중플레이어명령은같았지만실제지면궤적은인원수에따라달랐으므로순수성능대조로쓰지않는다.
+
+미확인: 앞서 기록된 actor61 원래4접촉 입력이 이번에 동일하게 재현됐다는 증거, 모든 sparse episode의 물리 sweep별 원인, 녹화/로깅 없는64성능, 최종GPU표시. 새로운 실패가 있으면 해당ID·세대·후속경로를 원문과 대조하고 기존 현재 구현과 격리 후보에서 원인 한 요소씩 시험해야 한다.

## 5. 검증 수준

- 이 묶음은 보존 B8FB의 기능 재관측이며 새 gameplay수정 효과 검증이 아니다.
-8/180은 원래 실패를 보존.300초8/64는 표에 따라 개별 판정. 성공한 일부 실행으로 전체 안정성을 선언하지 않는다.
- 정상 낙하·복구와 상태/출구/재접근 연결은 측정된 범위에서 진단 성공. 판정하지 못한 원인은 미확인으로 유지한다.
- 애니메이션 부드러움·지원 clock 배포·반복64수용·성능 최적화/20ms p95·최종 골: 미완료.

## 6. 변경 사항

기존 빌드 B8FBAB746F50CB7C8BDBB23E059C308298CEA5F57B4AC6BD0F2DDEBC55B8C185 유지, 미션/게임코드/PBO/개인키 변경 없음. 실행3회마다3개 기존 PBO 서명 검증 성공. 신규 PBO·재서명·배포 없음. archive 실행기·HIGH_HOLD 녹화 gate·분석/terminal-window·집계를 추가했다. 최초180초 결과/원문 분석을 덮어쓰지 않고 별도300초 결과와 연결했다. 운영 서버·공용 D61D·기존 증거는 보존했다.

## 7. 원시 증거

plan·추가시험 adjustment·세 launch reference, native-ground/natural-player/state-contract/traversal-events/admission/egress/pose-cadence/drive-cost·원본 source snapshots·서명 검증 출력·정상MKV·inspection sheet·분석기 실행log·개체별 terminal-window·batch-results를 기존 profile/이번 폴더에 보존한다. evidence hash·JSON·파일 안정성·정상 종료를 검증 후 기존 checkpoint/TXT·B안에 게시한다. 공개본은 선정 집계·관련 함수/대표로그이며 전체raw/영상·주소·로컬경로·프로세스·개인키를 제외한다.

## 8. 잔여 문제

이번에 남은 최종/중간/계약 실패가 있으면 해당개체의 원시전이와 support/contact/목표변경을 연결하고, 복구 중인 움직임과 미해결 정체를 구분해 최소 변경 후보를 만든다. 성능은 별도 통제 실험에서 actor/host수·계측조건을 맞추고 inclusive비용 중첩을 분리한다.이 batch의p95를 최적화 성공이나native/GPU/GC 비용 귀속으로 쓰지 않는다.

애니메이션의 입력dt는 native시간과 일치한다는 최신 진단을 유지하고, 확인된 overshoot reset·배포가능 setter/최종표시 계약을 별도 조사한다. 임의clock/보간이나 정상낙하 제거로 기능/성능 성공을 만들지 않는다. 새로운 분기에서 필요해진 도구/자료·중단 기준은 다음 격리 계획에 기록한다.

선정 공개 본문. 로컬 보고서 SHA-256: `49b5f54343f078f8dfb80fa53cf9f5dbc6a0069c319d8ef9cee9cc427ceb8096`. 원시 전체가 아니다.
