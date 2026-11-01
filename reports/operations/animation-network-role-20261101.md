# 실제 네트워크 mode2 계측: 그래프 진행 확인·포즈 동등성 미검증

작성: 2026-11-01T16:11:32.013110+09:00

## 1. 작업 목표

네트워크 클라이언트의 mode2 경로를 실제로 실행해 원격 개체 생성, 실제 모드, retained buffer, 그래프 진행을 확인한다. 이후 포즈·요청 동등성까지 검증된 경우에만 오프라인 시간 손실과 비교한다. 기존 NPC 권한 구조나 정상 밀침·낙하·복구는 수정하지 않는다.

## 2. 실제 수행

기존 서명된453D/0C6E/B8FB 자산으로 loopback 전용 서버와Steam 실행 client를 사용했다. 완전한8파트 NPC가 아닌65본 물리 native part1개를 ECE_CREATEPHYSICS로 원격 생성했다. 서버 요청은run=2,루트 이동OFF다. 근처 플레이어는 무적이며 기존 NPC 추적·전투 로직은 실행하지 않았다.

초기 시도는 경제 초기화 중 클라이언트를 먼저 실행해 제한 시간 내 접속·개체 생성이 성립하지 않았다. 해당 기록을 보존했다. 다음 시도는 미션 OnUpdate marker 뒤 client를 시작하고 경제 초기화를 제외했으나0x00010001 unreachable 화면이 확인됐다. Windows 컴퓨터 유즈 스킬로 오류창을 확인하고 작은 스크린샷1개를 로컬 보존했다. 메뉴 입력은 원격 개체 관찰로 이어지지 않았고 별도 성공으로 세지 않는다.

로컬 질의 포트27955는 A2S 요청에157바이트로 응답했다. 게임 포트에 같은 질의를 보낸 무응답을 게임 포트 불통 증거로 사용하지 않는다. 설치된 scripts.pbo의 DayZGame.Connect는 질의 포트가 지정됐을 때 IP:gamePort:queryPort 형식을 native Connect에 전달한다. 세 번째 시도는 준비 대기 조건·미션·PBO를 유지하고 CLI 주소만 그 형식으로 바꿨다. 새 포트 개방·방화벽 변경은 하지 않았다.

각 실행 전에3개 PBO 해시·서명을 검증했다. network listener는loopback,이번 격리 설정의verifySignatures=0이며 로컬 서명 검증과 네트워크 강제 검증을 구분한다. 운영 서버 설정은 보존했다. 관찰기는 기존 Python Frida이며4개 함수 바이트를 검증하고 native 값을 바꾸지 않았다.

## 3. 측정 결과

총3회는 조건을 보정한 실행 가능성 시험이며3회 반복 timing 비교가 아니다. 처음2회는 개체 미선택·서버DONE 없음,세 번째만 플레이어 생성·개체 생성·DONE이 확인됐다. 세 client 모두 창 닫기 요청 후 RPT termination marker 및 프로세스 부재가 확인됐다. 기존 오프라인 하네스의 retirement 계약은 수행하지 않았으므로 그 검증을 통과했다고 표시하지 않는다. 서버는 소유 프로세스만 계획된 Stop-Process로 닫았다. crash log0,관찰 오류0·스크립트 관찰 오류0이며모든관찰기가종료됐다.

세 번째에서 실제 mode2·65본 Animal1개를 선택했고 live vtable/subobject vtable이 이전 정적 자료와 일치했다. scheduler/buffer/consumer/interpolation 각각1273호출,매3호출후표본424개,선정 interpolation표본47개다.424개 모두mode2·primary50ms·secondary200ms·retained buffer 있음이었다. 보간 반환RVA0xb704de는 직전 정적 slot+0x48 소비 경로와 일치했다.

전체표본 QPC25.3485071초 동안 graph clock은25.3498502초 진행해 비율1.000053이었다. 첫5초 제외 탐색 분석은20.3478176초/20.3498344초·1.000099였으며 사후 제외 조건이므로 사전 수용 기준처럼 쓰지 않는다. 기존 오프라인0.833 관측과 파트 수·장면·역할이 달라 개선율을 계산하지 않는다.

매3 scheduler호출 간 표본간격 p50/p95/p99는60.307/76.344/78.219ms다. 이 값은 개별 frame-time·CPU self-time·Present가 아니다. 보간 numerator/duration의 선정47표본은0.1592~2.0이며8개가1을 넘었다. 초기 버퍼 상태와 변화를 포함하므로 이 결과를 완벽한 보간이나 화면 부드러움으로 해석하지 않는다.

서버 pose TSV2152행은 모두실제request2였다. client RequestedState는 읽지 못했고 client pose matrix 및 실제 애니메이션 화면은 측정하지 않았다.8/64 회귀·CPU/GPU/할당/GC·실제 frame-time p50/p95/p99도 이번에 미측정이다.20ms p95는 목표로 유지한다.

## 4. 원인 분석

확인: 네트워크 client에서 별도 mode2 buffered 경로가 실제로 실행되고 공유 하류 보간 소비자로 전달된다. 그래프 clock의 장시간 진행은 이 실행 범위에서 벽시계에 근접했다. 이는 정적 가능성만 남았던 경로를 실제로 확보한 진전이다.

추정: 준비가 된 같은 서버 조건에서 질의 포트를 포함한 주소로 바꾼 뒤 처음 접속했다. 설치 소스 형식·질의 응답과 양립하지만 단일 쌍이고 반복 검증하지 않아 접속 실패의 모든 원인을 질의 포트로 확정하지 않는다. 첫 실패의 경제·준비 조건도 동시에 달라 비용 원인 비교에 쓰지 않는다.

미확인: client가 서버와 동일run 포즈를 재생하는지,buffer 초기 과도구간이 화면에서 어떤지,8파트 동기화와 정상 NPC 권한·물리가 유지되는지다. 그래프 clock 비율1에 가까운 사실만으로 원본 애니메이션·시각 품질·성능 문제 해결을 선언하지 않는다. 기존 offline mode0 잔여시간 폐기·CPU/화면 연결 증거는 그대로 유효한 별도 조건이다.

## 5. 검증 수준

원격 생성·실제 mode2·buffer 전달·graph 진행은 실행 진단 성공이다. 요청 및 포즈 동등성 gate는 미완료다. 시각 품질·8/64 기능 회귀·성능 개선·반복 검증·최종 수용은 통과하지 않았다. failed attempts를 삭제하거나 성공 수에 포함하지 않는다.

## 6. 변경 사항

기존 research 안에 작은 연결용 미션·실행기·bounded 관찰기·3개 profile·분석본을 추가했다. 게임 코드·PBO·ANM·9350 후보·운영 서버·개인키는 보존했다. 기존3개 PBO는3회 실행 전 모두 서명·해시 검증 성공,새 PBO가 없어 새 서명 대상은 없다. 새 영상·대용량 dump·툴 설치는 없다.1개 오류 화면은 로컬에만 보존한다.

## 7. 원시 증거

실행 ID animation-network-role-20261101-1600. initial/ready-gated-trial/query-address-trial의 launch,network-observer,json/js,서버 poseTSV,profile과 plan/preflight-change/results를 연결한다. installed-login-contexts/installed-connect-contexts는 실제scripts.pbo 선택 행과 member 해시를 보존한다. 관찰은 REA runtime Evidence가 아니라 기존 Python Frida 결과다.

공개는 집계·선정 event projection·함수 문맥·실패 분류로 제한한다. 실제 process/PID/CLI·ASLR base·전체 로그·질의의 전체 응답·키·로컬 경로·화면은 노출하지 않는다. 프로세스 종료·JSON·해시·파일 안정성 확인 뒤 저장한다.

## 8. 잔여 문제

다음은 성공한 연결 형식과 미션을 재사용해 client의 요청/graph state 및 CPU 포즈가 서버와 대응하는지 확인하는 것이다. 이미 얻은1회 clock 진단을 최적화 성공으로 다시 포장하지 않는다. 동등성이 성립하면 그 뒤에만 기존 오프라인과 맞춘 입력·파트·씬·표시 비교를 설계한다.

client pose/request 접근 경로를 현재 원시 자료와 지원 getter에서 먼저 확인하고,부족하면 필요한 관찰만 추가한다. 전체 NPC 네트워크 권한 이식·새 보간·native mode 쓰기·속도 배율은 보류한다. 해시/서명 불일치·crash·관찰 오류·대상 이탈은 중단 조건이다. 사용자 추가 조작은 현재 필요하지 않으며 클라이언트는 종료돼 있다.

선정 공개 본문. 로컬 보고서 SHA-256: `d077083f2c4f9eb865b6a7893af493c0b62428d4fd871ff6e0e5c5614479f69c`. 원시 전체가 아니다.
