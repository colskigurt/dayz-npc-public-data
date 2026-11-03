## 1. 작업 목표

한 줄 그래프 범위 후보가 정상 이동·밀침·낙하·착지·FOLLOW·재등반을 보존하는지 기존8→64 씬에서 확인한다. 네트워크 오류 수정과 군집 기능·시각 품질·성능 성공을 분리한다.

## 2. 실제 수행

기존 고정 입력 replay8/64 하네스를 복제해 같은 서명 그래프 후보만 추가했다. 기존9PBO, 물리·AI·접촉·애니메이션 알고리즘과 각 규모의 원래 씬/입력 바이트는 유지했다. SupportContext 후보는 로드하지만 활성 flag는OFF다.8명 기능 게이트 통과 후64명을 실행했다. 시작 전 각각10PBO 해시·서명과64명512파트 초기화를 확인했다. 새native hook·영상·PBO는 없다.

종료 후 전이 sequence, 이전/다음 phase, 복구generation, 개체별 종료와 실제 milestone을 기존 분석기로 대조했다. 분석기는 미션의 정확한 trace header와 새candidate build ID만 반영했다.64명 시작 가드는 빈 문자열 사전 키를 포함한 유효JSON을 PSCustomObject로 읽지 못해 처음 거부됐으며, AsHashtable로 키를 보존해 수정했다. 실제 게임 시작 전의 파서 실패 기록을 남겼고8명 성공 조건은 완화하지 않았다.

닫힌 기존 frame/command clock 자료와 RPT를 재분석했다. 핵심4소스가 실제 로드된 원본 서명PBO와 byte 일치함을 확인하고, EntryAlign timeout·Backoff와 Failure label 유지 구조를 대조했다. 실험과 소유 프로세스가 종료된 뒤 저장했다.

## 3. 측정 결과

|항목|8명·64파트|64명·512파트|
|---|---:|---:|
|독립 실행, 각 규모|1|1|
|유효 종료 label|8/8|40/64|
|전체 이동 이력 충족|8/8|41/64|
|FALL 시작 / LAND→FOLLOW 연결|10/10|76/75|
|상태 전이|152|1068|
|phase/generation/terminal 연결 오류|0/0/0|0/0/0|
|기능 게이트|통과|실패|
|서버 Mission dt p50/p95/p99|16/31/32ms|73/116/141.92ms|
|명령 누적시간 / 미션시간, 개체 중앙값|0.999808|0.643184|
|명령 callback당 전달시간, 평균|약50ms|약50ms|
|client outside-landscape 경고|1|1|
|새 crash dump|0|0|

서버dt는 Mission OnUpdate의 시간 간격이며 client Present·CPU busy·GPU가 아니다. 명령시간은 DGNativeCommand Age가 PreAnimUpdate pDt를 더한 값이며 native graph clock·CPU bone·화면 시간도 아니다. 같은NPC 파트의 script시간·update·request 표본 spread는0이었지만 화면 동기 증명으로 사용하지 않는다. 실제 Present/CPU/GPU p50/p95/p99·할당·GC·반복 비용 비교는 미측정이며20ms p95는 목표다.

64명 종료 outcome은 VALID_TERMINAL40, INCOMPLETE_MILESTONES23, UNRESOLVED_STATE1이었다. phase는FOLLOW62·LAND1·MOTION1이다.40명 중1명은 실패 이유label이 남아 있어, 비어 있는label까지 요구한 보수적 집계는39명이다. 기존 분석기의 비유효 outcome 또는 남은failure 표식25명을 따로 검토했다.25명 모두 마지막20초에 accepted step 증가와10cm 넘는 표본 위치 변화가 있었다. 그러므로25명이 전부 영구 정지했다는 설명은 하지 않는다. 이동에는 밀침도 포함돼 목표를 향한 유효 진전이라고 자동 판정하지 않는다.

빠진 누적 이력은 낮은 곳0명·높은 곳7명·그 뒤 지면복귀7명·재등반23명이다. 이력 미충족은 현재 경로가 영구 단절됐다는 뜻과 다르다. ID3은274.807초 FALL,275.269초 LAND 이후280.064초 종료까지FOLLOW가 관측되지 않았다. 남은4.8초 관찰만으로 영구 복구 실패라고 단정하지 않는다. 현재 조건의 기능 실패는 그대로 보존한다.

## 4. 원인 분석

확인: 범위 후보 이후 이번8/64 전체 실행에서 반복 outside-landscape 경고 흐름은 관측되지 않았고 client 시작 경고가 각각1건이었다. 앞선 exact bad-ID hex 문자열0건만으로 경고 제거를 판단해서는 안 된다. 실제 경고는 packed object bitfield 숫자를 출력하므로 이번에는 실제 메시지 전체형식을 셌다. 로그 건수나 크기는 CPU self-time/FPS 개선률이 아니다.

확인:64명에서 command 시간이 mission 시간의 약0.643배로 진행했고 callback당 평균은 약50ms다.8명은약1배였다. 변경하지 않은 이동/회전은 callback dt를 사용하고 retry·정렬 기한은 mission/게임 시간을 참조한다. 두 시간영역이 벌어지면 진전과 기한 사이에 불균형이 생길 수 있다는 구체적 가설을 얻었다. 그러나 per-callback 입력과 각 timer/heading/접촉 결과를 연결하지 않았으므로 이번 정체의 확정 원인이라고 하지 않는다. client 재생이0.643배였다는 주장도 하지 않는다.

확인: EntryAlign의진전 기준은각도3도 개선,기한은전체5초/진전없음2초이며 평행이동 setback 진전은 이 기준에 없다. Backoff는연결을차단·재시도하고 failure label을유지한다. 따라서남은label과현재실패상태를구분한다. 종료기한이나milestone을늘리거나label을지워성공으로만들지않았다.

미확인:64명실패가범위수정의회귀인지,기존고정입력조건의성능·혼잡문제인지분리되지않았다. 원본그래프와의동일64입력기능비교는이번에없다. 이전적응형입력64/64와이번고정입력40/64를직접빼서회귀율로만들지않는다. 지지·관절질의는기존병목후보이나이번dt만으로순수비용을귀속하지않는다. 이미실패한NOCOLLISION호스트레이어후보는재적용하지않는다.

## 5. 검증 수준

네트워크 sentinel 범위: 기존1개체·8파트직접진단통과유지.8명기능:단일회귀통과.64명기능:실패보존.상태연결감사와명령시간차이:진단성공.독립반복성능·실제시각품질·14클립최종수용·20ms p95·전체goal완료:미달이다.정상낙하를일괄실패로세지않았지만현재정의된시험의기능실패는유지한다.

## 6. 변경 사항

새PBO를만들지않았다. 후보8638218523E0A568A75FCDCC7C21033F6D733DCD4D51D259639E87042839FD84의앞선자동서명·검증을유지하고각실행전10PBO를재검증했다. 새것은격리미션·런처·분석·보고서뿐이다. 원본소스·바이너리·기존PBO·Steam·운영서버·개인키는변경하지않았다. 골격·클립·충돌검사를삭제하거나시간을강제보정하지않았다.

서버는DONE후소유하네스정리,client exit-1이며normalQuitVerified=false를유지한다. 새dump0을일반정상종료인증으로바꾸지않는다. 기존보고서·실패원시기록은덮어쓰지않는다.

## 7. 원시 증거

sourceRunIds: network-gameplay/slot-bounds-functional-eight1 및 slot-bounds-functional-sixtyfour1. plan·mission/replay해시·launch/signature·시작gate·종료/전이/62,272행표본·milestone·serverframe/commandclock·RPT·분석을로컬해시에연결한다. failure-triage.json과source-contract-byte-proof.json이실패해석의근거다. 수치분석은동일원시자료의재분석이지새독립반복이아니다.

공개는규모별집계·논리ID·시간영역·잔여문제만제공한다. 절대경로·프로세스정보·게임자산·개인키·원시대용량로그는공개하지않는다. 관리자worker준비파일이없고현재프로세스가관리자가아님을확인했다. 기존ETW승인요청은대기중이며권한우회·서비스설치·그룹변경은하지않았다.

## 8. 잔여 문제

다음비교는이미구현·진단한상태제한SupportContext후보를단일요인으로재사용하는방향이다. 그래프범위수정과씬·입력을고정하고8명안전성부터확인한뒤64명OFF/ON을같은조건반복비교한다. 적격구조물에서4개추가ray를줄이는기존후보의기대이점은있지만nativeAPI추가guard비용과실제이득은별도측정한다. 기존검사결과강제true·충돌삭제·거리만으로정밀도축소는하지않는다.

판정을바꾸는데필요한값은기능종료·낙하복귀,후보실제사용/생략ray,서버dt/command시간,승인된경우실제Present/CPU/GPU다. 계산이줄어도기능이깨지면수용하지않고,한번좋은결과로최적화성공을선언하지않는다. 기존로그만으로정렬원인을고르지못하면각도·setback·timer를선택ID에만유계관측한다. 근거없이timeout을완화하거나새보간/dt보정방식을적용하지않는다.

로컬checkpointSHA256: 4db913faf327becce5db6c3cd6b68051afab1a0adec8d680812900efea7b9498
