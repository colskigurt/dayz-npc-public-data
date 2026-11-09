# Present 최소 수집 시도 — 장면 진입 전 실패
기준일: 2026-11-10 KST. 검증 수준: 실행 경계 진단 성공, 수집·회귀·반복 검증·최종 수용 미실행.

## 1. 작업 목표
기존5파트64 장면에서 10초 Present CSV 생성 가능성만 확인한다. 성능 비교나 기능 개선으로 판정하지 않는다.

## 2. 실제 수행
기존 서버/Steam 요청/장면 제어를 재사용하고 별도 출력 경로 client-present-viability-20261110을 준비했다. 서버·클라이언트·미션·13 PBO 해시가 기존 기준과 일치했다. 이전 start/stop 플래그는 복사하지 않았다. 단일10초 설정의 구문/ValidateOnly를 통과했다.
관리자 수집기 PID26676 준비를 확인하고 서버33464·클라이언트35572를 시작했다. Unity 채팅과 실행 충돌을 조율했다. 클라이언트 창을 확인했지만 준비 gate가 열리지 않아 Present 요청을 보내지 않았다.

## 3. 측정 결과
64개체 장면 준비 및 workload 시작 미확인. PresentMon 미실행, CSV 없음. 8/64 기능·CPU·Present p50/p95/p99·애니메이션 시각 품질 모두 미측정.
클라이언트 RPT에 00:46:21.676 netcrypt connected, 00:46:24.18 BattlEye initialization failed, 이후 반복 session terminated - no packets를 확인했다. 서버는 UDP30960/30962/30975 바인딩이 관찰됐다. CPU 소비나 프로세스 생존을 준비/성능 성공으로 간주하지 않았다.
Project Zomboid 창도 관찰됐으며 전용 성능 조건으로 사용할 수 없다. 해당 게임에는 입력·종료를 하지 않았다.

## 4. 원인 분석
확인: 이번 정체는 CSV 생성 이전의 장면 진입 단계다. 기록된 BattlEye 초기화 실패 및 패킷 부재는 단서다.
모름: BattlEye 메시지가 이번 로딩 정체의 유일 원인인지, Steam 동시 실행·접속·환경과의 인과관계.
기존 CSV 미생성 실패와 같은 원인으로 합치지 않는다. 방화벽·BattlEye·서비스·보안 설정을 변경하거나 우회하지 않았다.

## 5. 검증 수준
입력 검증·관리자 수집기 준비·실행 경계 진단 성공. 수집기 신규 기록 분기의 실제 Present 실행은 아직 미검증이다. 성능·기능·시각 품질 성공 및20ms p95 달성 없음.

## 6. 변경 및 종료
게임 코드·PBO·미션·Unity 무변경. 새로운 배포·서명 없음.
00:48:58 클라이언트 정상 창닫기로 종료. 소유 서버는 준비 전 정체 상태에서 강제 종료했으며 exit -1을 기록했다. 정상 서버 종료로 보고하지 않는다.
장면/관측 제어 두 작업을 중단하고, 준비만 하던 관리자 수집기는 PID·생성시각 대조 후 종료했다(00:49:31). 관련 네 프로세스 부재를 확인하고 Unity 채팅에 점유 종료를 전달했다.

## 7. 원시 증거
실행ID ANIM-present-viability-20261110-five64.
기존 weighted224-probe1/gameplay1/client-present-viability-20261110 아래 preflight.json, staged-plan.json, collector-config.json, admin-ready.json, executed-controller-sources.json, collector-retirement-request.json, collector-retired.json.
cost64-screen1/five 아래 plan.json, server-launch.json, steam-launch-request.json, client-closed-before-capture.json, server-forced-retirement.json, server-terminal.json 및 client/server-profile 로그.
종료 후 클라이언트 RPT SHA256: 0A54C2EF8590DB277339ECE03DD6CCD293253669D03A98FACCFC053428A061BC.
원시 자료와 기존 보고서는 덮어쓰지 않았다.

## 8. 다음 작업
기존 정상 Steam 실행의 동일 초기화 메시지 유무 및 접속/실행 방식 기록을 먼저 대조한다. 이 최소 차이가 확인되기 전 같은 장면을 반복 재시작하지 않는다. 기존 단일224 서버CPU 개선 근거는 유지하되 클라이언트/Present/64 전체 수용으로 확대하지 않는다.

관련: [수집기 진단 보완](operations/present-collector-diagnostics-20261110.md), [기존 수동8 결과](operations/single224-manual-review-20261109.md).
