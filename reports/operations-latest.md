## 1. 작업 목표

같은 평지·카메라·플레이어 입력 조건에서 바닐라64와 현재 검증 빌드 커스텀64의 전체 비용을 비교하고, 기존 애니메이션 시간 전달 문제와 구분한다. 최적화나 새로운 보간·물리 검사 생략은 구현하지 않는다.

## 2. 실제 수행

기존 바닐라 측정 씬·입력·카메라·PresentMon 및 커스텀93507d 실행 계층을 재사용했다. 새 진단 모듈729f3b는 평지에서 기존 Director/Body/Traversal/Crowd/Pursuit를 연결하고 위치 commit과 기존 누적 카운터를 관측한다. 구조물/등반 링크를 생성하지 않는 빈 fixture를 사용한다. 알고리즘·native graph·골격·전환 duration·simulation LOD를 바꾸지 않았다. 기존3개 커스텀 PBO와 공통 observer는 해시를 대조했다.

준비 중 예약어/복잡한 문자열 표현/지역 이름 충돌의 컴파일 거부는 별도 실패 기록으로 보존했다. 멀티플레이 pilot4에서는 커스텀 AI가 CHASE여도 nativeSeconds/animUpdates/moved 모두0이었다. 이를 성능 자료로 사용하지 않았다. 클라이언트 로컬 Animal의 실행 역할 차이 후보이며 원인을 확정하거나 강제 패치하지 않았다. 양쪽이 실제 실행되는 기존 오프라인 모드로 비교 환경을 통일했다. 이전 네트워크 바닐라 수치를 이 A/B에 섞지 않았다.

오프라인8마리 예비 시험: 양쪽8/8이 목표를 인식하고 실제 이동, 커스텀 native callback/포즈 시계 증가,64파트 안전 퇴역과 정상 종료 확인. 이후 독립 프로필에서 같은 소스·PBO로64마리 본 시험2회를 실행했다. 각 실행은0→V64→C64→C64연속→V64이며 C2/C3는 같은 개체가 이어지는 두 창이다. 독립4회 커스텀 반복으로 부풀리지 않는다. 각 fresh stage에서 플레이어 시작 위치·heading·이동 입력 시계를 초기화하고, 카메라와 환경은 유지했다. 물리 상호작용으로 실제 궤적은 달라질 수 있다.

PresentMon2.6 서명/해시, 관리자 principal, 해당 클라이언트의 식별/생성시각/프로필을 검증했다. 입력 추적·native hook·영상·덤프는 추가하지 않았다. Computer Use로 실제 씬을 확인했다. 모니터 약0.25초, entity trace 실제 약3.6~3.9Hz이며 구간별 실측 빈도를 JSON에 제공한다.

## 3. 측정 결과

각 populated stage의 첫7초와 마지막5초를 제외한 약33초를 QPC로 선택했다. 경계는 host가 관찰한 약0.25초 해상도이며 exact pose/display join이 아니다. ETW 대상·단일 swapchain·QPC/Present 간격 identity·foreground·기록 완료를 검증했다. 프레임 p50/p95/p99, 표시 주기, CPU/GPU busy/wait, script dt, process CPU를 JSON에 별도 제공한다. process CPU는 여러 스레드 합계라1000ms/초를 넘을 수 있다.

| 실행/구간 | Present p50/p95/p99 ms | CPU busy p95 ms | GPU busy p95 ms | 전체 프로세스 CPU ms/벽시계초 |
|---|---|---:|---:|---:|
| 1/empty | 19.949/26.234/33.766 | 14.749 | 1.789 | 1219.17 |
| 1/vanilla64 | 19.864/24.734/30.671 | 13.671 | 1.910 | 1236.28 |
| 1/custom64_fresh | 39.081/67.605/79.470 | 67.426 | 9.293 | 1524.64 |
| 1/custom64_continued | 38.573/72.382/80.037 | 71.401 | 10.006 | 1499.17 |
| 1/vanilla64_repeat | 19.862/24.549/28.502 | 13.790 | 1.889 | 1252.32 |
| 2/empty | 20.007/23.835/30.931 | 12.225 | 1.726 | 1241.46 |
| 2/vanilla64 | 19.917/24.619/29.276 | 13.342 | 1.926 | 1203.43 |
| 2/custom64_fresh | 40.005/67.879/75.879 | 67.370 | 9.369 | 1475.03 |
| 2/custom64_continued | 39.510/71.782/78.932 | 69.981 | 8.243 | 1500.71 |
| 2/vanilla64_repeat | 19.886/24.172/26.496 | 12.592 | 1.915 | 1170.21 |

두 독립 실행에서 V/C 모두64/64가 목표를 인식하고2m 이상 이동했다. 커스텀 분석 행은 FOLLOW,8파트/논리개체를 유지했다. 이것은 평지 추격 관찰 통과이며 도착 terminal/동시등반/혼잡 전체 회귀를 뜻하지 않는다. vanilla displacement와 custom Moved는 각각 초기점 변위와 누적 이동량이라 크기를 직접 비교하지 않는다. 사망/예외/정상 퇴역 여부는 집계와 원문 로그로 분리했다.

아래 값은 기존 TickCount 중첩 범위의 경과 환산 ms/게임초다. 동일 실행 stage clock으로 unsigned32 wrap을 처리해 환산률을 실측했다. 벽시계와의 근소한 양자화·경계 오차를 포함하며 CPU self-time이 아니다. 부모·자식 및 mission/PrePhys 경로가 중첩되므로 합산하지 않는다. legacy drive는 모든 실행 경로를 포괄하지 않는다.

| 실행/커스텀 구간 | mission update | 접촉 | 발 지지 | 월드 스윕 | 본 조회 | pursuit | perception |
|---|---:|---:|---:|---:|---:|---:|---:|
|1/2|289.56|201.51|110.20|69.27|9.29|92.73|58.82|
|1/3|302.66|182.26|100.54|60.48|8.99|107.88|58.26|
|2/2|285.51|209.33|115.00|72.49|9.22|88.41|58.51|
|2/3|301.21|190.27|105.52|63.61|8.99|108.73|58.20|

정확한 호출/초와 position/orientation write 수는 customScopes에 있다. SupportSweepFast=true지만 일반 지형에서 StructureSupportBound가 성립하지 않으면 full foot 검사를 유지한다. 실제 발 지지 ray/solve=10, 관절 sweep/solve≈4는 이 경로와 일치한다. 빠른 경로 플래그만으로 검사 생략 효과를 보고하지 않는다.


CPU 애니메이션 시계 실측(게임1초당 nativeSeconds / leader callbackHz):

- 실행1/C2: 0.7638초 / 15.276Hz.
- 실행1/C3: 0.7417초 / 14.834Hz.
- 실행2/C2: 0.7570초 / 15.140Hz.
- 실행2/C3: 0.7403초 / 14.807Hz.

## 4. 원인 분석

확인: 같은 오프라인 환경에서 커스텀의 프레임 및 CPU busy 지연이 반복 증가했다. GPU busy도 증가하나 CPU busy가 프레임 간격과 가까워 CPU 측 실행이 주요 비용 후보다. 이것은 특정 함수의 causal self-time 확정이 아니다. 커스텀64는512개 native Animal/애니메이션 파트이고 바닐라는64개 native infected다. 논리 개체 수가 같아도 실행/렌더 작업량은 다르다.

확인: 기존 스크립트 범위에서 발 지지·관절 월드 검사는 충돌용4본 조회보다 크고 pursuit/perception/social/crowd도 별도 비용 후보로 남는다. mission update 범위는 native callback을 전부 포함하지 않는다. 발 지지와 월드 범위를 줄이면 전체 차이가 사라진다는 주장은 아직 미검증이다.

애니메이션 시계: 각 logical leader의 nativeSeconds 증가율과 callback Hz를 독립 측정했다. 실제 callback 전달 시간의 손실은 CPU 출력 시계에 관한 직접 관측이다. 과거mode0의50ms 클램프/누적기 reset 원인 증거와 양립하지만, 이번에는 엔진을 hook하지 않았으므로 현재 실행에서 동일 명령 경로가 원인임을 새로 확정하지 않는다. 전체 프레임 처리량 저하와 시계 손실은 연결 가능성이 있으나 같은 지표가 아니다. 그래프 내부 평가·최종 renderer 소비 포즈·실제 mesh 표시 cadence는 이번 ETW로 측정되지 않았다. 스크린샷과 CPU callback 증가를 시각적 smoothness 검증으로 대체하지 않는다.

추정: 8파트의 중복 native 평가·표시 commit과 높은 접촉/추적 호출 빈도가 함께 비용 차이에 기여할 수 있다. 반증: CHASE 상태만으로 정상 실행이라고 본 MP pilot 가정은 실제0회 callback에 의해 기각했다. 바닐라/커스텀의 순수 animation self-time 차이, drawcall/skin/upload/GC, 개체별 LOD/가시성 및 전체 비용 중 고유 비율은 미확인이다. 바닐라는 native 전투 가능, 커스텀 Combat=false이고 실제 충돌 궤적도 다르므로 완전 동일한 연산 부하라고 하지 않는다.

## 5. 검증 수준

진단 성공 및 같은 조건 독립2회 재현: 평지 V/C 비용 차이, 실제 추격, CPU animation callback 시간 비율, 퇴역·정상 종료. 기능 검증은 이 평지 씬에 한정한다. 기존 등반·낙하·착지·피격·사망 전체 회귀는 이번 범위 밖이다. 시각 품질 최종 수용은 미실행, 성능20ms p95 목표는 미달이다. 최적화 후보는 구현·효과 검증하지 않았다.

## 6. 변경 사항

진단 전용 bridge PBO SHA-256729f3b883bac5951acf011b706a93e3437a4a9031d1e9a205f830cd911242ebf. packed member/source 일치, 서명 및 모든 실행 전 서명 검증 성공. 기존93507d와 두 resource PBO 및5d5fc2 observer는 변경하지 않았다. 개인키 배포 없음. 새 offline mission/launcher/monitor/analyzer만 추가했다. 두 본 시험의 mission source 해시는 동일하다. 운영 서버·DayZ 원본·기존 배포 PBO는 수정하지 않았다.

exit code -1은 DONE/RPT 정상 termination/crash 부재를 함께 확인해 판정했다. 두 client 강제 종료 없음. 기존 diagnostic plugin script(E)8줄 및 resource/startup/shutdown 경고를 보존했다. 추가로 종료 시 PGFilter64개와 BunkerBroadcastManager1개의 leak 경고가 두 본 실행에서 관찰됐다. 예비8개체에서는 PGFilter8개였다. 이전 network vanilla-only baseline에는 BunkerBroadcastManager1개만 있었다. 정상 종료와 별개로 memory cleanup은 미수용이다. PGFilter 생명주기 소유권이 후보이나 원인을 확정하지 않았다. warning-free라고 보고하지 않는다. 기존 보고서/최신 인덱스/B안 미러를 재사용한다.

## 7. 원시 증거

배치matched-flat-pursuit-20261102. offline-pilot,offline-main,offline-main2의 launch/mission-used/client-frames/custom-agents/custom-costs/vanilla-agents/stages/script/RPT와 본 시험 host-observations/present/admin-completed/analysis를 로컬에서 보존한다. 원문은 덮어쓰지 않고 저장 전 SHA-256/파일 크기/mtime/JSON parse/안전 종료를 검사한다. 공개는 집계·자체 fixture/observer 소스만 제공하고 로컬 경로·프로세스 정보·raw 전체 CSV·비공개 자료는 제외한다. 준비 실패 프로필은 별도 보존하며 정상 반복 수에 포함하지 않는다.

## 8. 잔여 문제

다음 최소 실험은 두 갈래다. 성능: 같은 flat harness에서 observation OFF 대조로 observer 부담을 먼저 경계 설정하고, 기존8파트 native 실행과 script contact/추적 비용을 한 요인씩 분리한다. 검사 생략은 진단용 격리 조건에서만 검토하고 현재 기능 빌드에 적용하지 않는다. 애니메이션: 기존 단일 개체 시간 전달/실제 표시 포즈 연결 연구를 유지한다. 이번64 결과를 단일 개체 끊김의 유일 원인으로 소급하지 않는다.

종료 leak 경고의 소유권·reference 정리 원인은 남은 문제다. 실행 중 메모리/GC 증가를 계측하지 않았으므로 실행 중 누적 누수량으로 해석하지 않는다. 네이티브 graph/skin/upload/GC의 세부 CPU self-time은 현재 Enforce 계측에서 확보하지 못했다. 표시 시간은 프레임 단위이며 개체별 애니메이션 포즈를 구분하지 못한다. 관측 OFF, 순수 native 평가 분리, 실제 영상/표시 포즈 시간 계측을 수행하기 전에는 특정 검사/8파트 변경을 해결책으로 확정하지 않는다. 이번 결과를 이유로 새로운 보간·타임 스케일·LOD·충돌 삭제를 적용하지 않았다.

로컬 checkpoint SHA-256: 7ad207a4687b159f1567a3046315cab6a3b932eb7eb90d87d1267682d7287842
