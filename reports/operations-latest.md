# 64개체 실제 표시 계측 확보: CPU쪽 비용 후보,정렬 복구1건 잔여

작성: 2026-11-01T12:37:18.227333+09:00

## 1. 작업 목표

기존7E16의64개체 workload에서 실제 CPU/GPU/Present/표시 비용을 측정하고 전체300초 상태 계약과 독립 판정한다. 과거 실패한ETW 자료를 재사용하거나 성공으로 고치지 않는다.

## 2. 실제 수행

2026-11-01 12:25에 준비된 관리자 도우미를 OS RunAs로 실행했다. UAC 클릭이나 보안 설정 변경을 자동화하지 않았다. 실제 관리자 principal·live PID/생성시각·준비 시각·도구 해시를 대조한 뒤 격리 클라이언트 runtime-20261101-122619를 실행했다. 기존3종PBO 서명 검증은 모두 성공했다. 새PBO·게임 코드 변경은 없다.

미션8.084초에 요청을 기록했고 도우미는8.844~39.070초 LOW_HOLD에서30초만 PresentMon2.6을 실행했다. 다른ETW 세션 중단·서비스 설치·권한 그룹 변경·키보드 추적은 하지 않았다. helper/tool/client 모두 종료됐고 전체300초 시험도 명령 retire와 mission 종료를 거쳐 정상 종료됐다.

초기 상태 안내에서 시작 파일/프로세스 목록을 충분히 확인하지 않고 도우미 정체로 판단한 오류가 있었다. 실제 기록은 요청접수29.151ms 뒤 도구 시작,30초 후 exit0이었다. 잘못된 관찰과 정정 근거를 별도JSON으로 보존했다. 이 성공이 과거 도우미 정체 원인을 설명하지는 않는다.

## 3. 측정 결과

실제 Present631건, 단일DXGI chain, display timing626건(5건 값 없음), QPC상 Present 표본 범위29.780초다. helper 측정 경과30.204초, 미션 구간235표본 모두LOW_HOLD/low였다. display timing 없음만으로 drop5건이라고 단정하지 않는다. Present모드는 Hardware Composed Independent Flip617건·Composed Flip14건, SyncInterval1은631건이다.

| 측정값 | p50(ms) | p95(ms) | p99(ms) |
|---|---:|---:|---:|
| MsBetweenPresents | 48.222 | 93.070 | 115.190 |
| MsBetweenDisplayChange | 40.044 | 95.006 | 120.027 |
| MsCPUBusy | 45.144 | 91.088 | 105.656 |
| MsCPUWait | 0.143 | 16.290 | 19.370 |
| MsGPUBusy | 6.666 | 12.207 | 13.838 |
| MsGPUWait | 40.942 | 85.302 | 105.996 |
| MsUntilDisplayed | 15.783 | 24.969 | 28.140 |


위 수치는30초 실제 표시 계측이다. 전체미션 script renderDt10095표본은 p50/p95/p99=22/65/94ms, 같은 미션 bracket의 script p95=90.900ms다. 서로 다른 시간 계층·범위이므로 숫자를 합치거나 NPC 포즈 프레임으로 취급하지 않는다.20ms p95 목표는 미달이다.

기능:64/64 유효 도달,미해결0,낙하87/87 착지·추적재개,등반종료 실패0,물리전환 대응통과,low/high/ground FOLLOW 각각64,foreground1.0이다. 하지만 ENTRY_ALIGNMENT_NO_PROGRESS1건 때문에 기존 엄격 algorithm-label gate는 실패했다. CONTACT_SUPPORT_LOST25건은 정상 물리 전환과 복구에 연결했다. 착지 continuation GRANTED0회로 새lease 효능은 여전히 미입증이다.

16번의 정렬 watchdog은48.374~48.923초 사이 확인됐다.51.971초 FALL_REQUEST의 같은 호출 detail은FOLLOW_AREA_OR_FLOOR_GUARD이고,이전ENTRY_ALIGNMENT 라벨이 failure필드에 남아 있었다.52.794초 FOLLOW로 복귀했고 최종ARRIVED다. 정렬 backoff와 나중 낙하의 원인을 동일시하지 않으며 기존 엄격 gate도 그대로 실패로 둔다.

## 4. 원인 분석

확인:이 구간에서는 CPU frame-work 시간이 GPU active 시간보다 길고 GPU wait가 크다. GPU계산만으로93ms급 Present 간격을 설명하기 어렵다는 방향 근거이며 CPU/동기화 쪽 귀속을 우선한다. CPUBusy는 PresentMon 프레임 시간이지 전체 스레드의CPU self-time이 아니다. GPUwait로 특정 스크립트가 원인이라고 단정하지 않는다.

같은bracket의 inclusive elapsed는 발지지 130.98,관절·월드 83.32,충돌본 조회 8.08ms/벽시계초다. global 인식 53.45,crowd 27.24,pursuit 0.47ms/벽시계초다. Director카운터는id0에서 한번만,개체 접촉은64개 rate로 읽었다. 범위가 중첩되므로 합산/self-time 해석을 금지한다.

| 귀속 대상 | 확보한 측정 | 남은 제한 |
|---|---|---|
| 프레임/표시 | script·QPC·실제Present/display 분포 | NPC별 표면/포즈 갱신과 동일하지 않음 |
| AI·경로·군집 | 기존 global inclusive counter/frequency | 개별 native 함수 self-time·병렬 대기 분리 안 됨 |
| 발지지·관절/월드 | 기존 body inclusive counter | parent/child 중첩,실제 검사를 끈 비교 아님 |
| animation graph/native 평가 | 미측정(runtimeTimingProbe=false) | 값0으로 해석하지 않음 |
| render/GPU | process GPUbusy/wait·Present 경계 | bone palette upload·draw별 비용 없음 |
| allocation/GC | 미측정 | 현재 Enforce/PresentMon 자료로 분리 불가 |

가설:한번0으로 기록된 align bestHeading과 접촉으로 흔들리는 실제 heading,wall watchdog/pose settling의 관계가 정렬 backoff에 영향을 줬을 수 있다. 현재 기록만으로 원인을 확정하지 않는다. 기존2초/5초 가드를 무작정 늘리지 않는다.

## 5. 검증 수준

실제64 ETW 수집·CSV/QPC 정합·LOW bracket은 진단 성공.64최종 도달·87낙하 복구는 관찰 통과. 기존 엄격 기능 gate는 정렬 watchdog1건으로 실패. 시각 품질·최적화 효과·반복 검증·20ms·최종 수용은 미완료다. 다른 빌드/AI조건의1개체 ETW와 비교해 개체수 효과나 개선률을 계산하지 않는다.

## 6. 변경 사항

기존7E16/453D/0C6E를 그대로 사용했고 서명3/3을 실행 전에 검증했다.새서명 대상과 개인키 배포는 없다. 기존 실패 capture·원시 기록·공용D61D·운영 서버를 보존했다. 분석기는 기존 parser/상태 계약/비용 분석기를 재사용했고 새 ETW bracket·같은 호출 원인 분리 파생물을 추가했다. 이 시험은 native hook/영상 없이 진행했다.

## 7. 원시 증거

sourceRunId presentation-etw-worker-20261101-0712,실제 runtime-20261101-122619,실행 요청12:26:19.623+09:00. admin-capture-20261101-122714의 present.csv·capture-closed,worker의ready/start/completed/closure,기능·lease·phase·alignment-case16 파생물과 원시 contract/ground/entry trace를 연결한다. CSV SHA256=983a1f54567d88226b139b0cd12ea24aa77070a490f2acb4223a455a306fe6e0. 저장 전 기존sourceHashes·파일 안정성·JSON·프로세스 종료를 검증한다.

필드 정의는 [PresentMon v2.6.0 공식 문서](https://github.com/GameTechDev/PresentMon/blob/v2.6.0/README-ConsoleApplication.md)를 참조했고 받은 본문의 hash/선정 정의를 별도 기록했다. MsAnimationError를NPC animation오차로 사용하지 않는다. 공개에는 선정 집계·대표원인만 제공하고PID/프로세스 인자/로컬 경로/키/raw 전체CSV·영상은 게시하지 않는다.

## 8. 잔여 문제

먼저 이 진단 묶음을 안전 종료 상태로 B안에 갱신한다. 다음은 정렬 watchdog의 실제 발생과 이후 failure 라벨 전달을 분리하고 기존 고정 fixture에서 heading/위치/pose-settle 진행 조건을 대조한다. 필요하면 작은 가역 후보를 분리하되 정상 밀침·낙하·복구와 엄격 판정 원본을 보존한다.

CPU 세부 범위는 현재 inclusive counter만으로 확정하지 않는다. 기존격리 fixture를 활용한 최소 한 요인 비교가 필요하다. 안전 검사를 운영 경로에서 삭제하거나 새 보간·배속/LOD를 추측 적용하지 않는다. 계측ON/OFF 오버헤드·동일조건 반복·개별 화면 포즈 실측·독립 원본/ANM 충실도는 별도 미완료로 유지한다.

선정 공개 본문. 로컬 보고서 SHA-256: `9d7dc95bf5e3b25f83dedbf8c5758de169f87e081de4f9e43dd7b27d65acb7fb`. 원시 전체가 아니다.
