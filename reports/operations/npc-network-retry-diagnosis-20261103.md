## 1. 작업 목표

기존64개체 network 후보의 비용 문제에서 반복 네트워크 ID 조회를 분리한다. 지원/접촉/낙하 검사를 없애지 않고 실제 반복 소유자와 안전한 수정 가능성을 찾는다. 현재 애니메이션 mode2 개선과 전체 비용을 혼동하지 않는다.

## 2. 실제 수행

닫힌 기존8/64 RPT를 스트리밍 분석했다. A500 진단 실행 파일에서 메시지·제한된 코드 구간·실제 caller를 대조하고, 별도 짧은 격리 실행에서 조회3건 및 레코드3건만 읽기 전용 Frida 계측했다. native 게임 필드에는 쓰지 않았다. 실제 caller의 live RTTI 접두부는 NetworkClient, 레코드 객체는 DayZAnimal이었다. RTTI 긴 원문은 인접 메타데이터가 섞여 있어 첫 타입 접두부만 해석했다. 레코드가 부모/계층 연결인지 아직 확정하지 않았다.

REA의 기존 Frida binding 누락을 확인하고 같은17.23.2 공식 prebuilt를 복구했다. persistent MCP import cache는 실패를 유지해 fresh CLI의 실제 Evidence로 수집했다. MCP attach 성공으로 바꾸지 않는다. 첫 Ghidra startup은900000ms backend timeout으로 종료됐고, 후속 단일 함수0x140a12460 분석은2시간 startup deadline으로 실행 중이다. 분석기 생략/재시작/강제중단은 하지 않았다.

공식 SDK ECE_SETUP의 full NEW entity setup 의미를 근거로, 기존 Initialize에서8파트 생성 flag에만 SETUP을 추가하는 default-OFF 후보를 격리했다. ECE_CREATEPHYSICS 및 내부28쌍 차단·외부충돌은 유지하고 INITAI는 추가하지 않았다. 같은 새PBO·같은1NPC/8파트 미션에서 OFF/ON을 각각1회 실행했다. 공통0.5초 CPU 포즈/물리 존재 관찰기는 두 조건 모두 사용했다. 새 영상·덤프는 없다.

## 3. 측정 결과

기존64 RPT318146844bytes 중 outside-landscape 경고3345674행, 최대13312행/초였다. 이전 contact-end-lift1에서도3345803행으로 새 SupportContext가 처음 만든 현상이 아니다. 기존64 서버 RPT 해당 경고0행이다. 로그 건수/크기는 순수 CPU 비용이 아니다.

|조건|파트/물리/머리 포즈 변화|감사 구간|경고 총건수|초당 최대|새 crash 파일|
|---|---|---|---|---|---|
|SETUP OFF|8/8, 모두true, 모두변화|0.3~8.00799초|1593|200|0|
|SETUP ON|8/8, 모두true, 모두변화|0.3~18.1261초|3593|200|0|

실행 길이가 다르므로 총건수 비율을 효과로 계산하지 않는다. 두 조건에서 경고가 계속됨을 확인했으며 SETUP 추가만으로 해결된다는 후보는 미수용이다. CPU 머리 변화는 실제 표시 부드러움 측정이 아니다. 게임은 시작대기 fixture에서 퇴역시켜 종료했고 추적/등반8→64 회귀 통과로 세지 않는다. client exit-1, server owned cleanup; 정상quit 검증false. p50/p95/p99·Present·CPU busy·GPU·할당량·GC는 이번에 미측정이다.20ms p95는 목표다.

## 4. 원인 분석

관찰 사실: static-ID lookup은 해당ID에서 null을 반환하는 경로에 있었고, NetworkClient loop가 같은 미해결 레코드를 재조회했다. lookup hook3건 중 detach 이전 반환2건이 직접관찰됐고3번째 반환은 누락됐다. 다른 hook3건은 모두 NetworkClient/DayZAnimal 타입접두부와 같은caller였다. 각3건은 독립 실험3회가 아니다. 메시지 숫자는 packed ID bitfield이므로 NPC가 거대한 월드좌표로 이탈했다고 해석하지 않는다.

정적 해석: 현재 bounded disassembly에서 loop는 null이면 항목을 남기는 분기로 간다. 레코드 producer·ID lifecycle 의미·어느파트의 참조인지·전체 CPU 비용 기여는 미확정이다. allocator 관련 helper가 경로에 있어도 malloc 횟수/바이트/self-time을 확보했다고 하지 않는다. 경고 숨기기나 null을 강제로 성공 처리하지 않는다.

반증 범위: full entity setup 추가가 이1NPC 대기 조건의 반복경고를 없애지는 못했다. SETUP 자체가 모든 상황에서 무의미하거나 다른후보가 모두실패라는 결론은 아니다. 애니메이션 시간 손실의 기존 인과증거와 mode2 시각 개선 관찰을 이번 네트워크 오류 하나로 대체하지 않는다.

기존64 failure history 재검토: CLIMB_EXIT_VALIDATION4행은1회복구 에피소드가 라벨을 유지한 것이다. 해당ID50의 첫 LAND101.499초, 이후 첫 FOLLOW107.259초였다. 예전 분석 followAt130.839초는 같은generation의 나중FOLLOW로 덮어쓴 값이다. 첫복귀 latency에 사용하면 안 된다. 총82/82 복귀 판정은 바뀌지 않는다. EGRESS_STALL은 Head/컨테이너 접촉 후 유한탈출 종료·재계획 이력이며 원인을 정상밀침으로 단정하지 않는다. 원본 분석/실패기록은 보존했다.

## 5. 검증 수준

로그 규모·실제 caller/타입 경로: 제한 진단 성공. SETUP 후보: 빌드/서명/실제compile·파트존재 확인, 경고제거 실패로 보류. 안정성8/64·시각품질·성능·반복검증·최종수용은 이번 묶음에서 미실행이다. 기존 support-context8/64 기능 결과는 그대로 보존한다. 전체 골active다.

## 6. 변경 사항

격리 nf_native_setup PBO SHA256=9C6C4DF3E6BE0C14B8A63C4BCD05B11A55347DFDCE8A1742FFE60854FD30029F. 자동서명·DSCheckSignatures 및 packed2members 대조 성공, 실제engine World/Mission compile·실행 확인.10개모듈 서버 hash/서명을 시험전 확인했다. 기존9PBO·원본자산·운영서버·Steam·native시계·이전보고서는 변경하지 않았다. 개인키 미포함. 빌드파일의 engineCompiled=false는 빌드당시 상태이며 실제 실행증거는 각후속 launch/RPT다. 후보는 미채택이며 기본OFF다.

## 7. 원시 증거

기존 network-gameplay 아래 outside-landscape-log-audit·static/caller/owner 분석JSON, outside-log-stack1/2 및 outside-log-parent3의실패/성공CLI Evidence·종료launch, native-setup-candidate2의source/plan/build/sign/packed, native-setup-off1/on1의launch/RPT/TSV, native-setup-controls1-analysis를 해시에 연결한다. 원시전체는 로컬에보존하고 공개에는 집계만 노출한다. 현재 Ghidra 출력은 진행중이라 불변증거에 포함하지 않고, 해당시점프로세스 liveness snapshot만 보존한다. 기존 회복기록정정은 context-failure-history-review.json에 있다.

## 8. 잔여 문제

다음 단일산출물은 미해결 network record의 producer→조회→해제 조건을 연결한 수정가능성 판정이다. 실행중 decompile은 중단하지 않는다. 연결의미를 확정하기전 소스수정/복제해제/부모강제설정/로그차단은 하지 않는다. 수정가능 국소원인이 나오면 먼저동일조건1NPC에서ID/physics/pose/lifecycle을 비교하고 정상밀침·낙하·착지·FOLLOW·재등반8→64 및 matched비용/실제화면을 별도판정한다. SETUP후보에는8→64확대를 하지 않는다.

로컬checkpointSHA256: c1161e783f245215b101e04cd9472b40578539fe83b76242d12edfbe1ae74fcb
