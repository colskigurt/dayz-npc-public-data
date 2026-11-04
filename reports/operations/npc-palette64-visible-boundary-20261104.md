# 5파트 키 보존과 단일 파트 실제 표시 경계

## 1. 작업 목표

5파트 재분할의 안정적 native 표시 검증으로 가는 실행 경로를 복구하고, 원본 키/계층 보존을 확인한다. CPU 최적화 목표를 기능 복구나 Steam 설치 작업으로 대체하지 않는다.

## 2. 실제 수행

기존 subset 검증기를 재사용해5파트×14클립을 메모리 안에서 pruning·재파싱했다. 출력70클립은 만들지 않았다. 필요한10개 접촉/루트 본과 모든 조상을 유지했다.

사용자가 일반 DayZ는 정상 실행됐다고 확인했다. 인자 없는 기존 DayZDiag도 같은 Steam 오류를 화면에서 재현하여 모델 입력과 분리했다. bounded REA .NET metadata 조사에서는 런처 identity/선택 타입·문자열을 읽었고,확실한 diag 선택 옵션은 확인하지 못했다. Ghidra 초기화·엔진 전체 분석은 하지 않았다. 필요한 결과를 보존하고 REA를 닫았다.

이후2026-10-04 Steam 경유 접속 성공 기록과 기존 start_horde_sandbox_client.ps1을 찾았다. 이를 먼저 재사용하지 않아 직접 실행 실패를 반복한 점을 기록한다. Steam -applaunch 경로는 정상 retail client를 실행했다. diag server와의 첫 접속은 실행 종류 불일치로 거부됐다. 설치된 release dedicated server로 동일 fixture를 실행했고,초기 메뉴 재접속의unreachable은 목적지가 입증되지 않아 서버 호환성 실패로 집계하지 않는다. 새 client의 명시적 loopback 초기 접속에서2파트가 생성됐다. BattlEye 재시작 안내 후 정상 재시작을 한 번 했으나 안내가 다시 발생했다. 보안/검사 우회는 하지 않았다.

## 3. 측정 결과

|항목|결과|
|---|---:|
|키/부모 계층 정적 보존|70/70조합|
|5파트 요구 본 합계|401|
|신규 클립 출력|0|
|새 server role 실행|4(독립 성능 반복 아님)|
|새 retail client|3|
|인자 없는 diag control|1|
|network3 서버 pose 행|1042,2타입|
|network4 서버 pose 행|828,2타입|
|실제 모델 화면|단일 snapshot1개|

화면 왼쪽은 기존48 palette part0,오른쪽은 새64 palette part0다. 서로 얼굴/면 범위가 다른 부분 모델이다. 한 프레임에서 오른쪽 머리·상체·팔 일부의 표시를 확인했고 극단적인 메시 폭발은 보지 못했다. 전체 몸·모든64 영향 본·모든 면·seam·14클립·연속 부드러움을 통과시킨 것은 아니다. 사진은 비공개 증거로 보존했다.

p50/p95/p99·전체 CPU 이득·8/64 기능 회귀는 미측정/미실행이다.20ms p95 미달성.

## 4. 원인 분석

확인:Steam 경유 retail 실행은 정상이며 직접 diag 실행은 모델 없이도 Steam 오류를 보였다. 관리자 실행 차이를 현재 원인으로 확정할 근거는 없다. 등록PID 불일치만으로 원인을 확정하면 안 된다는 과거 성공 기록도 재확인했다.

확인:release server1.30.164200.56과retail client1.30.164289.70은 이번 fixture에서 연결·character 생성·2모델 spawning까지 진행했다. 숫자가 다르다는 이유만으로 사전 실패 처리하지 않았고 전체 프로토콜 동등성으로 일반화하지 않는다. client의 BattlEye update 로그와 반복 restart-required 안내가 있다. update가 근본 원인이라는 인과는 미확정이다. 첫 오류를 자산 탓으로 처리하지 않는다.

## 5. 검증 수준

정적 키/계층 보존:진단 성공. Steam retail launch와 이번 release fixture 접속:제한 진단 성공. 전체5파트 native 표시·안정적 연속 관찰·GPU skin 정확도·8/64·성능·최종 수용:미완료. source native pose 변화와 실제 화면 관찰을 구분한다.

network3은60초 pose 수집과retirement DONE을 확인했다. network4는서버180초 wall deadline 때문에DONE 이전 종료되어 정상 완료로 보지 않는다. 두 client는 메뉴에서 닫았고RPT 종료 성공 표식·프로세스 부재를 확인했다. exit code는보유하지 않는다.

## 6. 변경 사항

새 게임 코드·모델·PBO·clip 변경 없음. 이전에 서명 검증한 단일 파트 PBO를 그대로 사용했다. 변화는 격리 mission/role/Steam launch 조건과 읽기 전용 audit·집계·세 장의 작은 screenshot이다. Steam 설치·registry·운영 서버·계정·보안 설정·개인키는 변경하지 않았다. staging network2의UTF8 읽기 실패는 process 생성 전 실패로 보존하고 실제 실행 수에 넣지 않았다.

## 7. 원시 증거

palette-budget-review-20261104/inmemory-key-contract.json 및palette64-steam-network1/2b/3/4의plan,launch,pose TSV,client RPT/terminal observation을 연결한다. 인자 없는diag control은Computer Use live error observation으로 기록하며 hook/메모리 쓰기는 없다. selected REA metadata는 비공개 file에 보존했다.

visible-part0-frame0.png와restart 안내 이미지는 로컬 비공개 증거이며 공개 packet에는 file ID/SHA와관찰 한계만 제공한다. 사용자/Steam 식별자·로컬 절대경로·전체RPT·원본 모델·PBO·키는 공개하지 않는다. 원본 실험을 덮어쓰지 않았다.

## 8. 잔여 문제

다음 실행 조정은 기존 공식 런처에서 정상 지원하는 client 초기화/재시작 경로 확인으로 한정한다. recognized -useBE 문자/필드는 정적으로 관찰했지만 해결 효과를 검증한 옵션은 아니다. 보안 차단을 우회하거나 무한 재접속하지 않는다. 안정적 표시가 확보되기 전 전체70클립 생성·5파트 gameplay 승격·8/64 장시간 시험을 하지 않는다.

5파트 정적/컴파일/CPU pose 근거는 보존하고 실제401본 resource 연결과 전체 mesh 검증은 미완료로 유지한다. 기능 개선을 추가하지 않고 CPU 성능 목표도 달성으로 보고하지 않는다.
