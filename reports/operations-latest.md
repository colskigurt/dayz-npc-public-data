# 월드 이탈의 BattlEye 경고 포착: client 실행 상태 확인 필요

작성: 2026-11-01T16:42:35.347356+09:00

## 1. 작업 목표

네트워크 포즈 시험에서 클라이언트가 중간에 월드를 떠나는 이유를 확인한다. 애니메이션 오류, 인증 플래그, 사망, 계측 조건과 실행 환경의 문제를 구분한다. 기존 NPC 구현과 시간 손실 근거는 보존한다.

## 2. 실제 수행

설치된 scripts.pbo에서 서버 ClientDisconnectedEvent의 실제 필드인 logoutTime/authFailed를 확인했다. 격리 미션에 이 값과 플레이어 생존·체력을 기록하는 override를 추가하고 기본 처리는 그대로 호출했다. 사용자 ID는 새 진단 이벤트에 넣지 않았다.

동일한 1파트·고정 run·루트 이동 OFF·서버 여섯 본 조회 조건으로 client matrix OFF 1회, ON 1회, ON+이탈 화면 자동 저장 1회를 실행했다. 각 실행의 client 창을 컴퓨터 유즈로 앞으로 가져왔으나 시점이 달랐고 지속적인 appActive 계측은 없었다. 따라서 엄밀한 무작위 반복 A/B가 아니다.

관찰기 counter를 0.5초마다 저장하고 1.5초 이상 갱신이 없으면 gap marker를 남겼다. 마지막 실행에는 이 신호를 기다리는 bounded Windows Graphics Capture 관찰을 미리 연결해 창이 닫히기 전에 화면을 저장했다. 첫 ON 실행의 수동 화면 확인은 창이 이미 종료돼 실패했고, 그 실패를 보존했다.

## 3. 측정 결과

matrix OFF는 1,261회 호출·420표본·유효25.1167022초로 관찰 구간의 25초 gate를 충족했다. 그러나 관찰 비활성화 약2.474초 뒤, NPC 생성 약30.695초 뒤에 월드 이탈이 기록됐으며 서버 DONE보다 약14.028초 앞섰다. 이 실행도 전체 세션 안정성 통과로 보지 않는다.

matrix ON은264회·88표본·유효5.204623초, ON+화면 저장은877회·292표본·유효17.4420918초였다. 두 경우 모두 2초 counter gap과 서버 disconnect 이벤트를 확보했다. 두 이탈은 각각 NPC 생성 약8.348/20.677초 뒤이며 서버의 예정 종료보다 앞섰다.

세 실행의 서버 disconnect 값은 모두 authFailed=false, alive=true, health=100, logoutTime=15였다. 이는 해당 이벤트의 인증 실패 플래그나 사망을 원인으로 삼을 근거가 없다는 뜻이며, 플랫폼 인증·연결 전반이 정상이라는 증명은 아니다.

마지막 실행 화면에는 Warning 0x000400F0, You were kicked off the game, BattlEye, Game restart required가 표시됐다. gap marker부터 화면 저장까지 같은 host wall clock 기준 약0.621초였다. 이는 frame-time이 아니다. 설치된 ClientKickedModule.c는 BATTLEYE=240과 해당 표시 접두사를 정의하며, 주석에는 admin kick에도 쓰일 수 있다고 되어 있다. 이번 격리 시험에서 admin kick 명령은 요청하지 않았다.

관찰 오류0·스크립트 관찰 오류0·numeric override0·crash log0이다. 세 서버 모두 DONE 후 소유 프로세스만 닫았고, 세 client와 관찰기도 종료됐다. 기존 retirement 계약 검증은 아니다. 새 8/64 회귀, 화면 부드러움, frame-time p50/p95/p99, CPU/GPU·할당/GC는 측정하지 않았다.20ms p95는 목표다.

## 4. 원인 분석

확인된 사실: 마지막 월드 이탈은 게임이 표시한 BattlEye 분류의 kick이다. 포즈 좌표가 잘못돼 게임이 crash했다고 설명할 근거는 없다. 서버에는 BattlEye=0이 설정돼 있었지만, 그 설정만으로 client의 실제 anti-cheat 실행 상태까지 검증된 것은 아니다.

반증 범위: matrix OFF 실행도 관찰 종료 후 서버 정지 전에 이탈했으므로, 추가 matrix 읽기가 있어야만 이탈한다는 설명은 맞지 않는다. ON에서 더 빨리 이탈한 시간 차이를 CPU 비용이나 검출 민감도에 귀속할 근거도 없다.

미확인: 실제 client launch가 어떤 BattlEye 상태를 만들었는지, 어느 모듈·서비스·엔진 조건이 경고를 발생시켰는지는 아직 모른다. native 관찰과의 관련성은 검토 후보지만 이 화면 하나로 특정 훅·읽기 함수가 원인이라고 확정하지 않는다. 이전 모든 이탈을 같은 원인으로 소급 분류하지 않는다.

Launcher/게임 실행 파일의 문자열에는 BattlEye 설정 후보가 있지만 문자열 존재는 지원 인자나 실제 제어 효과의 증거가 아니다. 추가 native 연결 시험을 반복하기 전에 지원되는 테스트용 실행 방식과 실제 client 상태부터 확인해야 한다.

## 5. 검증 수준

서버 해제 이벤트와 마지막 오류 화면 포착은 진단 성공이다. 원인 범주가 구체화됐지만 정확한 실행 환경 원인 및 해결은 미완료다. matrix OFF의 25초 관찰은 전체 세션 수용·반복 안정성·시각 품질·성능 개선과 구분한다. 최종 수용은 미완료다.

## 6. 변경 사항

기존 research 안의 격리 미션 이벤트 기록, bounded heartbeat·gap 신호, 창 보존과 이벤트 화면 저장만 추가했다. 게임 코드·PBO·ANM·운영 서버·9350 후보는 변경하지 않았다. 기존3개 PBO 해시·서명을 세 실행 전에 모두 검증했고 새 PBO가 없어 새 서명 대상은 없다.

시스템 서비스·드라이버·보안 설정을 바꾸거나 anti-cheat를 패치하지 않았다. 새 영상·대용량 dump·툴 설치는 없다. 연결 중 화면과 오류 화면은 작은 로컬 PNG로 보존하며 공개하지 않는다.

## 7. 원시 증거

분석 ID network-continuity-20261101-1630. matrix-off/root, matrix-on-trial, matrix-on-capture-trial의 미션·launch·heartbeat·gap·native event·서버 pose TSV·profile을 보존한다. captured-error-diagnosis/results/results-timeline은 원본을 덮어쓰지 않고 별도 파생했다.

disconnect-script-contexts와 battleye-script-contexts에는 설치 PBO member 해시·행 문맥을 담았다. 실제 화면 SHA-256은76e480e7059bd98334c8c1a4b03421ee471412125a9845812fa71cf4ac9eefca다. native 관찰은 기존 Python Frida이며 REA runtime Evidence가 아니다. Windows UI 관찰은 computer-use 스킬의 sky API로 수행했다.

공개본에는 집계·이벤트 flags·상대 시각·화면 텍스트 전사·선정 소스만 담는다. 전체 로그·화면·PID·실제 CLI·ASLR base·사용자 식별자·로컬 경로·키는 제외한다. 종료·JSON·해시·파일 안정성 확인 뒤 저장한다.

## 8. 잔여 문제

다음은 지원되는 BattlEye 비활성 모딩 시험용 client 실행 방식을 읽기 전용으로 확인하고, 실제 상태를 검증하는 것이다. 검증 전에는 같은 native 연결 시험을 더 반복하지 않는다. 서비스/드라이버/보안 설정 변경이나 native anti-cheat 패치로 이 gate를 우회하지 않는다.

테스트 세션 상태가 확인되면 기존 name-hash→slot과 여섯 본 기록을 재사용해 연속성·동기 시각·포즈·표시를 다시 구분한다. 적절한 실행 경로가 확인되지 않으면 네트워크 분기의 한계를 기록하고, 이미 확보된 오프라인 mode0·64개체 비용 자료에서 진행 가능한 분석을 이어간다. 전체 NPC 권한 이식·새 보간·시간 배율 수정은 보류한다.

선정 공개 본문. 로컬 보고서 SHA-256: `504c8d9d9968364341a03f4520334659835c0d68194564c7bd94dee4bdef0899`. 원시 전체가 아니다.
