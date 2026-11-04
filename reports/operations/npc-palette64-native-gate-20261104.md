# 64본 단일 파트 native 검증 경계

## 1. 작업 목표

5파트 재분할 후보를 전체 구현하기 전에64 weighted palette 파트 하나의 native 컴파일·본 매핑·실제 표시 가능성을 확인한다. 기존8파트·물리·접촉 솔버는 유지한다.

## 2. 실제 수행

원래 visual/Memory 바이트를 보존한 단일 모델에 기존 Geometry LOD를 그대로 추가했다. 기존 전체250본·14클립 바인딩을 상속하여 새 클립 없이 진단했다. 별도 PBO를 빌드하고 packed ODOL/config.bin을 확인했다. 최초 서명 호출은 경로 인자 오류로 실패했고 canonical Windows 경로 인자로 서명·검증에 성공했다. 실패 기록도 보존했다.

격리 서버는 원본 대조와 후보의64개 본 이름을 읽고 같은 native animation 시각의 MS pose를 비교했다. 완료 표식·retirement 후 소유 서버를 종료했다. 그 뒤3개 격리 client 시도는 씬 진입 전에 Steam 인식 오류로 종료됐다. visible1은 사용자 일시정지 중 종료 상태가 미확인으로 남아 재개 후 프로세스 부재·오류 덤프를 확인했다. visible2는 알려진 launcher/graphics cfg/dependency set, visible3은 child-process App ID 지정으로 구분했다. 설정·계정·Steam 설치를 변경하지 않았다.

## 3. 측정 결과

|항목|결과|
|---|---:|
|컴파일 모델|ODOL,540089bytes|
|새 PBO 서명·검증|성공|
|배포 폴더 개인키|0|
|본 이름 매핑|64/64|
|native 관측|250행|
|활성·동일 시각 비교|222/222행|
|64본 최대 MS pose 차이|0m|
|대조/후보 native callback 최대|24/24|
|실제 렌더링된 후보 관찰|미실행|

위 포즈는 CPU native skeleton 출력이며 스키닝 픽셀 검증이 아니다. 새 모델은 전체250본을 상속한 단일 파트 진단이고 최종79본 subset이나5파트 통합본이 아니다. p50/p95/p99·64개체 비용·시각적 부드러움은 미측정이다.

## 4. 원인 분석

확인:64본 weighted MLOD는 현 binarizer에서 ODOL로 만들어졌고 native class 생성/본 매핑/원본과의 CPU pose 진행도 통과했다. 실제 화면에서는 모델 대신 Steam 인식 오류창을 확인했다. 처음 두 MDMP의 동일 오류 메시지도 확인했다. 이는 새 자산의 시각 실패나 성공 증거가 아니다.

사용자 제안인 관리자 실행 차이를 확인했다. 현 실행 도구와 Steam 모두 Medium·비상승,같은 계정·세션이며 DayZDiag manifest는 asInvoker다. 종료된 진단 프로세스의 실제 토큰은 읽지 못했으므로 그 사실을 과거 모든 실행에 소급하지 않는다. 강제 관리자 호환성 설정은 발견하지 못했다. 현재 근거로 관리자 불일치를 확정하지 않는다.

Steam UI는 실행 중이지만 읽기 전용 ActiveProcess 두 registry view의 pid/ActiveUser가0이었다. 연결 정보 문제의 후보이지 원인 확정은 아니다. 명시 App ID도 해결하지 못했다. 사용자 작업의 런처·서버가 이후 새로 실행돼 추가 client 실험은 진행하지 않았다.

## 5. 검증 수준

컴파일/서명/64본 CPU pose: 진단 성공. 실제 mesh skin·시각·반복 성능·8/64 기능·최종 수용: 미실행. 오류창 관찰을 애니메이션 관찰로 바꾸어 쓰지 않는다. 서버 exit−1은 완료 후 소유 종료이며 정상 종료 인증이 아니다. client 실패도 정상 퇴역으로 보고하지 않는다.20ms p95는 목표다.

## 6. 변경 사항

새 격리 nf_palette64_probe PBO와 전용 plan/mission/launcher만 추가했다. 운영 게임 코드·기존 PBO·Steam 설정·키 원본은 보존했다. 서명 검증 성공,개인키 공개/배포0개. 원본 Geometry LOD 바이트와 기존14클립을 유지했다. runtime launch의 inherited newPbo=false 필드는 해당 launcher 내부 생성 여부이며 이번 묶음의 새 진단 PBO 존재를 부정하는 근거로 쓰지 않는다. 실제 build/packed/plan을 함께 읽는다.

## 7. 원시 증거

palette64-part0-probe1/{plan,build,packed-proof,sign-first-failure,bundle-analysis},palette64-part0-runtime1/{plan,server-launch,pose TSV},palette64-part0-visible1/2/3의plan/terminal/launch/오류 dump를 연결한다. 공개 packet은 집계·선정 오류 메시지·해시만 포함한다. MDMP·원본 모델/PBO·계정 정보·로컬 경로·전체 process/환경은 공개하지 않는다. Computer Use는 DayZ 오류창을 직접 확인했으며 모델 화면 관찰은 없었다.

## 8. 잔여 문제

먼저 일반 Steam 실행 결과와 진단 전용 실행 문제를 구분해야 한다. 관리자 차이로 단정하거나 보안 설정을 바꾸지 않는다. 클라이언트 연결이 정상화된 후 단일 파트의 실제 스키닝을 확인한다. 그 전에는 전체70클립 변환·5파트 gameplay 통합·8/64회귀를 시작하지 않는다. 기존 기능과 실패 증거는 보존한다. 전체 CPU 절감은 아직 미입증이다.
