# 접촉 후보8 OFF/ON: 제한 통합 관찰, 전체 기능 미수용

## 1. 작업 목표
서명된 기본OFF 후보의 실제 네트워크8개체 연결과 접촉·낙하·상태 전환을 확인. 전체 기능/성능/시각 판정은 별도.

## 2. 실제 수행
동일12PBO·동일 혼합 지면/low/high 시간 자극과 고정카메라, 활성화 플래그만 변경. 각60게임초. 기존 상태 분석기 재사용. 서버 detach/RequestExit 후 종료, 클라이언트 정상닫기.
최초 분석의 모든행 read=true assertion은 초기 lazy정책8행씩 때문에 실패. 초기행 pair0을 확인하고 보존하며 첫pair 뒤 정책을 검증. 재실험하지 않음.

## 3. 측정 결과
|항목|OFF|ON|
|---|---:|---:|
|구간 게임초|60.022|60.0283|
|생존·인식·2m이동|8/8|8/8|
|관찰 렌더/receiver 생존|40/8|40/8|
|등반|27|26|
|drop|6|4|
|낙하 시작 및 착지FOLLOW복귀|11/11|8/8|
|상태chain/복구세대/terminal 정합성 오류|0/0/0|0/0/0|
|전체목표·valid terminal|6/8|5/8|

trace1856행씩, host480행씩. OFFfalse/ONtrue 정책 확인. 각조건1회, 경로와 타이밍이 달라 도착률 차이를 개선/회귀로 확정하지 않음.
전체CPU/Present p50/p95/p99·실제 표시품질·총기/근접/사망 최종수용 미측정.

## 4. 원인 분석
모듈 World컴파일·네트워크동작·실제 개체쌍 접촉 및 낙하복귀 사례 확인.
ENTRY_STALL_BACKOFF/CONTACT_SUPPORT_LOST가 양쪽 남고 OFF에는EGRESS_STALL도 있음. 모든 실패가 동일 기존 원인이라고 확정하지 않으며 새 회귀 없음으로 최종수용하지 않음.
기능복구 개발로 CPU최적화를 대체하지 않음.

## 5. 검증 수준
BOUNDED_INTEGRATION_OBSERVED_FULL_FUNCTION_NOT_ACCEPTED.
제한 통합 진단 성공. full8/64회귀·전체CPU·시각·배포 수용 아님.20ms달성 아님.

## 6. 변경 사항
격리 미션/profile/관찰/분석만 추가. 기존 배포본·게임소스 불변.
기존 후보PBO A226824ED32670C0B4145F3D2F2A1B2F8FEDACBE0FFAF40A47E57E2DB65573BF 재사용. 서명 검증 성공·개인키 미포함.

## 7. 원시 증거
ANIM-pair-shared-eight-integration1-off/on.
내부 gameplay-eight/mixed-hosts/state-contract/terminal/pair-policy-use, 로그·종료·functional-analysis/comparison-analysis 보존. 저장 전 해시 검증. actual server exit-1, 강제 종료 없음. 민감 원시 파일 공개 없음.

## 8. 잔여 문제
full mobility 미수용 유지.64 CPU-only 선별1쌍은 준비만 했으며 미실행. 사용자 우선순위에 따라 기존 결함과 비용을 분리하되 채택/배포 gate는 면제하지 않음.
40게임초 부근 동일 시작/30벽시계초, 시계읽기100ms, 갱신률/64생존 조건. 기존 수집기의2초 준비구간은 별도 기록.
8시험의 기존 observer는320기준이라1Hz열거가 계속되므로 클라이언트 성능 근거로 사용하지 않음. 실제 화면·피격/사망 최종검증 남음.
