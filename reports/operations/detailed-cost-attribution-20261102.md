## 1. 작업 목표

DayZ의 바닐라와 커스텀 감염자를 같은 평지·카메라·플레이어 입력 조건에서 비교하고, 기존 1/8/64개체 결과를 보존한 채 큰 비용의 실행 경계를 좁힌다. 이번 목표는 상세 진단과 다음 시험 준비이며 애니메이션 수정 완료나 20ms 달성 목표 완료가 아니다. Days Gone 관찰은 사용자의 요청대로 보류했다.

## 2. 실제 수행

기존 205초 하네스(빈 씬→바닐라64→커스텀64 fresh→같은 커스텀64 continuous→바닐라64 fresh)를 재사용했다. 커스텀은 논리64/네이티브512파트다. 정식 gameplay PBO는 93507d 버전을 유지했다. 새 진단 PBO bf40dd는 Drive/Displace별 SolveContacts와 바닐라 CommandHandler의 super 호출 전후에 카운터만 추가한다. 인자·호출 순서·반환값·물리 검사는 유지했다. OFF도 래퍼와 출력기가 있으므로 완전한 무계측 baseline은 아니다.

독립 실행은 OFF→ON→ON→OFF 네 번이다. 각 실행에서 PresentMon으로 프레임/CPU busy/GPU busy를 수집하고, V1/C2에 약8초씩 최소 CPU sampled-profile WPR을 적용했다. 전체 실행 환경에 로깅·PresentMon이 있고, V1/C2에는 WPR 시작/종료/병합 영향도 있다. C3/V4의 조용한 창을 별도로 비교한다. 도구는 기존 설치본이며 보안 설정·권한 그룹·서비스를 변경하지 않았다. WPR은 junction 경로를 거부하여 동일 폴더의 실제 경로로 실행했다. 전체 시스템 이벤트 원문은 비공개 보존하고 공개 파생본에는 시험 대상 집계만 담는다.

실제 바이너리 SHA가 같은지 검증하고 PE unwind 범위·Capstone·등록 문자열과 핸들러 연결을 읽었다. Ghidra 전체 자동 분석이나 원본 소스 복원은 수행하지 않았다. REA objdump 제한을 전체 네이티브 분석 불가로 확대하지 않는다. 1ms CPU 표본과 스택을 함수 범위 후보에 대응시켰다. 이는 정확한 호출 횟수·함수 self-time·개체별 비용이 아니다.

추가로 기존 실제 발판 제거 시험을 1개체로 실행했다. 새로운 알고리즘 없이 기존 fault harness, SimulationLod=0, 120초 조건이다. 계측 출력만 활성화하는 mission 어댑터를 사용했다. 첫 support1은 CFG가 없어 NPC 생성 전에 실패했고, 기록을 보존한 뒤 올바른 CFG의 support1b를 실행했다. 이 보조 시험은 평지 비교 성능 표에 합치지 않는다.

## 3. 측정 결과

평지 창은 각 구간 첫7초/마지막5초를 제외한 약33초다. 아래 p50/p95/p99는 PresentMon의 프레임 Present 간격(ms)이며 개체당 시간이 아니다.

| 실행 | 구간 | p50 | p95 | p99 | 판정 |
|---|---|---:|---:|---:|---|
|off64a|1 vanilla64|19.97|24.36|26.13|사용|
|off64a|2 custom64_fresh|40.71|68.79|76.35|사용|
|off64a|3 custom64_continued|39.51|74.74|81.76|사용|
|off64a|4 vanilla64_repeat|20.86|53.42|63.77|제외: foreground|
|on64a|1 vanilla64|19.94|24.24|27.26|사용|
|on64a|2 custom64_fresh|40.43|69.56|79.00|사용|
|on64a|3 custom64_continued|41.54|73.00|84.76|사용|
|on64a|4 vanilla64_repeat|20.06|24.43|26.96|사용|
|on64b|1 vanilla64|20.00|24.23|26.38|사용|
|on64b|2 custom64_fresh|39.07|65.77|75.93|사용|
|on64b|3 custom64_continued|39.53|75.30|87.61|사용|
|on64b|4 vanilla64_repeat|20.08|24.17|27.55|사용|
|off64b|1 vanilla64|20.01|24.44|26.66|사용|
|off64b|2 custom64_fresh|40.50|65.47|79.00|사용|
|off64b|3 custom64_continued|40.44|70.09|76.83|사용|
|off64b|4 vanilla64_repeat|19.89|23.92|26.02|사용|

off64a의 마지막 바닐라 구간은 약12초 foreground 이탈로 제외했다. 나머지 사용 창은 foreground 조건을 충족했다. CPU busy p95는 V1 약12.8~13.0ms, C2 약64.5~68.3ms다. 바닐라 Present p95도20ms를 넘는다. VSync/표시 주기와 CPU busy를 구분하며 CPU busy<20을 프레임 목표 달성이라고 하지 않는다. 이번 결과로 어떠한 성능 개선도 선언하지 않는다.

각 실행의 V1/C2/C3에서 목표 인식과 2m 이상 이동은 64/64였다. 커스텀 평지 표본은 FOLLOW였다. 이는 평지 추적 관찰이며 64개체의 수직 이동·전투·모든 terminal 상태 회귀 통과는 아니다.

ON 두 실행의 C2에서 Drive 경유 contact는 980~988회/게임초, Displace 경유 contact는 980~988회/게임초였다. contact 범위 경과는 각각 약105~110ms/게임초와98~103ms/게임초다. 상위 Drive는346~360ms/게임초, Displace는143~149ms/게임초였다. 서로 중첩된 경과 범위를 합산하지 않는다. 바닐라 이동 command101의 스크립트 CommandHandler super 경계는 약3108~3125회/초,25.4ms/초다. 이 작은 경계가 바닐라 AI·물리·애니메이션 전체 비용이라는 뜻은 아니다. 런타임 enum은101/104 등이며 최초 pilot의0~15 범위 가정은 기각하고 수정했다.

CPU 표본8개(네 실행 × V/C)의 유효 창은 약7.66~7.71초, 대상 표본 가중치는9585~11688, Lost Events/Buffers는 모두0이다. 스택 누락2~12, 첫 IP 불일치3~8개는 inclusive 분석에서 제외했다. 바이너리 내부 범위 매핑은 부분적이며 무심볼 unwind fragment가 포함된다.

| 실행/구간 | 대상 표본 | Ray inclusive | Sphere inclusive | scheduler inclusive | script 후보 leaf | allocation-calling 두 범위 leaf |
|---|---:|---:|---:|---:|---:|---:|
|off64a/1|9654|7|0|291|300|119|
|off64a/2|11667|1117|992|3541|1110|1789|
|on64a/1|9735|9|0|344|265|172|
|on64a/2|11662|1149|953|3511|1152|1788|
|on64b/1|9585|10|0|318|308|152|
|on64b/2|11621|1087|914|3424|1063|1686|
|off64b/1|9813|5|0|330|283|145|
|off64b/2|11688|1085|973|3465|1091|1706|

allocation-calling 두 구간의 leaf 표본은 on64b에서1686개, off64b에서1706개다. 이 중 Ray/Sphere 바인딩 스택과 겹치는 union은 각각1677개(99.47%)와1702개(99.77%)였다. 이 비율의 분모는 전체 CPU가 아니라 해당 두 구간의 leaf 표본이다. malloc/free 호출이 존재한다는 정적 사실과 연결되지만 실제 할당 횟수·바이트·GC 시간은 측정하지 않았다.

같은 ID0의 첫128개 contact 표본에서 인접 Drive→Displace42쌍은 기록된 anchor/heading/minimumLift 완전 일치가0쌍이었다. anchor 차이는 약0.00394~0.04190m다. 전체 개체의 결과가 다르다는 증거도, 같은 포즈 generation이라는 증거도 아니다. 무조건 동일 결과 캐시로 대체할 근거는 확보되지 않았다.

support1b는 MOTION3/FALL1/LAND6/FOLLOW226개 행을 남겼다. 실제 SUPPORT_REMOVED→RESTORED를 기록했고, climb1/landing1 후 ground FOLLOW/ARRIVED로 끝났다. failures1은 발판 소실 과정의 복구 사건과 함께 읽어야 하며 단순 terminal failure로 합산하지 않는다. contact bucket은 Drive-MOTION30, Drive-OTHER74, 직접-OTHER19회 등이다. EXIT_RECOVERY는0이지만 FALL/LAND가 OTHER에 포함되므로 복구 비용0이라고 하지 않는다. 낙하 프레임 통합18회가 기록됐다. 이는1회 보조 시험이며 별도 반복 검증 완료는 아니다.

## 4. 원인 분석

확인된 사실: 커스텀 평지에서는 이동과 개체 밀침의 두 contact 경로가 거의 같은 빈도로 실행된다. 물리 script binding을 경유한 스택이 바닐라보다 커스텀에서 크게 늘어난다. allocation-calling hot range 대부분이 그 물리 질의 스택에 있다. 따라서 단순한 본 수나 애니메이션 재생 비용 하나로 전체 차이를 설명할 수 없다.

강한 연구 후보: Ray/Sphere 질의 내부의 임시 결과 컨테이너·후보 처리 또는 관련 반복 작업. 두 hot range는 배열 크기/용량 형태의 필드와 malloc/free 호출을 가지지만 원본 형식·함수명·런타임 분기를 완전히 복원하지 않았다. ‘매 질의마다 할당한다’, ‘GC가 병목이다’, ‘이것만 고치면20ms’는 미입증이다. 다음에는 이 경로의 호출 수/입력별 후보량/상위 소유자를 좁힌다.

중요한 구분: Animal clock scheduler의 inclusive 표본에는 하류 스크립트와 물리 질의도 포함된다. 이를8파트 애니메이션 평가 순수 비용으로 취급할 수 없다. 바닐라 내부 물리는 같은 script RayCastBullet 바인딩을 통하지 않을 수 있으므로 해당 바인딩 표본이 작다고 바닐라 지지 검사 비용이0인 것도 아니다. query helper의 작은 표본이나 buffer dispatcher의0표본 역시 전체 비용 부재를 입증하지 않는다.

반증·한계: OFF/ON C3 p95는 OFF74.74/70.09, ON73.00/75.30ms로 실행 간 변동과 겹친다. 추가 카운터의 유의미한 성능 영향은 미확정이다. 예전1개체에서도 native clock 비율 약0.833이 관찰되어64부하만으로 시간 손실을 설명할 수 없다. 이번 계측은 실제 mesh 포즈의 화면상 시간적 연속성 측정이 아니며 새로운 보간·시간 배율을 적용하지 않았다.

## 5. 검증 수준

- 진단 성공: 호출자별 contact 분리, 실제 command ID, 손실0 CPU 표본·부분 스택 귀속, 실제 발판 소실 복구 관찰.
- 반복 검증 완료 범위: 평지 독립4실행에서 큰 바닐라/커스텀 비용 차이와 반복 hot range 확인. observer OFF/ON 각각2회지만 영향 크기 결론은 미확정.
- 기능 회귀 통과 범위: 평지64 추적 관찰 및1개체 지원 소실 복구에 한정. 64개체 전체 수직/전투 회귀 아님.
- 미실행/미수용: 실제 표시 포즈 연속성 정량 판정, native 모든 세부 self-time, heap/GC 직접 계측,20ms p95 최종 수용.

## 6. 변경 사항

gameplay93507d, resource2종, 공통 observer5d5fc2, bridge729f3b는 유지했다. 새 nf_detail_cost diagnostic build BF40DD6F99263DB7486D92FA502901FD41A80F1F0A67FF3A999141964561D6A4는 자동 서명 및 서명 검증에 성공했다. 각 실행 전에 모든6개 PBO의 해시·서명을 다시 검증했다. 개인키는 공개/배포에 포함하지 않았다.

수정·생성한 자체 소스는 detailed/source/nf_detail_cost의 NFDetailCounters.c/NFDetailWriter.c, 격리 mission/launcher, 최소 WPR helper, CPU 표본·접촉 분석 스크립트와 보고서다. 게임플레이의 보간/LOD/충돌/예약/정상 낙하를 변경하지 않았다. 기존 pilot compile 오류, 잘못된 command ID, 정확히 동시인 것으로 가정한 counter window 거부 기록도 보존했다. Counter 대조는 인접 기존 표본의 시간 bracket으로 수정했으며 동일 프레임 완전 동일성 증명으로 바꾸어 말하지 않는다. 정상 실행4회와 support1b는 클라이언트 퇴역·정상 종료를 확인했다. 기존 PGFilter/BunkerBroadcastManager 종료 누수 경고는 미해결이다.

## 7. 원시 증거

실행 ID는 matched-flat-pursuit-20261102/detailed/{off64a,on64a,on64b,off64b,support1b}다. 각 launch, 고정 mission, CFG, script/RPT, host 관찰, PresentMon CSV, 상세 TSV, cpu-1/2.etl, trace frequency/lost stats, 표본 파생 JSON, diagnostic-summary.json, native-boundary-candidates/followup 및 dispatch-pointer/import-v3를 연결한다. 실패 pilot와 support1도 그대로 보존한다. CPU v4는8개 실행 창, v6 ancestry union은 on64b/off64b C2 두 창이다. 같은 ETL의 재분석을 새로운 실행 반복으로 세지 않는다.

공개 evidence는 대상 집계와 자체 진단 소스의 선정 사본이다. ETL 전체, 다른 프로세스 식별자, 로컬 절대 경로, 설치 게임 자산, 개인키는 공개하지 않는다. 기존1/8 보고서·baseline64의 원문 해시를 다시 대조하고 새 증거는 저장 시점의 해시·안정성을 확인한다. 전체 원시 자료를 보존했다는 것이 모든 측정 항목을 확보했다는 뜻은 아니다.

## 8. 잔여 문제

이번 DayZ 비용 조사 묶음은 가능한 script/CPU 표본 경계까지 완료했다. 애니메이션 해결과 성능 최적화는 완료하지 않았다. 필요한 다음 실험은 다음과 같다.

1. 물리 질의 hot range의 상위 호출 소유자와 입력별 후보 결과량을 기존 ETL/제한적 계측으로 더 좁힌다. 발 지지·충돌을 통째로 삭제하는 시험보다 우선한다. API 내부 임시 컨테이너가 맞는지 확인 후에만 대체 결과 버퍼/국소 broadphase 후보의 실현 가능성을 검토한다. engine API가 고정이라면 script만으로 내부 할당을 바꿀 수 없다는 제한을 보고한다.
2.8파트 scheduling/pose/render 소비의 소유자를 분리하려면 객체 식별이 가능한 engine profiler 또는 검증된 native 경계 계측이 추가로 필요하다. 무심볼 scheduler inclusive 시간을 animation self-time으로 쓰지 않는다. 실제 화면 포즈 검증은 고정 카메라·단일 비이동 개체·CPU 포즈와 화면 프레임의 공통 시간 대응이 필요하다.
3.Days Gone은 이후 좁혀진 질문 하나만 확인한다. 사용자는 익숙한 챌린지에서 무적·탄약·노클립 후 지붕 한 위치에 대기하면 된다. 안정화 후 같은 시점으로 약30초 CPU/표시 기록을 수집하고 화면 내 개체 수·카메라·설정을 기록한다. 다른 엔진의 전체 비용과 DayZ 특정 함수 비용을 직접 뺄셈하지 않는다. 요구되는 원본 내부 경계가 없으면 전체 CPU/프레임/표시 주기 비교까지만 판정한다. 아직 실행·측정한 시험이 아니다.

원시 자료·기존 구조는 보존했고 새로운 캐시·GPU 방식·군중 설계를 적용하지 않았다. 이번 결과는 다음 실험 우선순위의 근거이며 최적화 성공 선언이 아니다.

로컬 checkpoint SHA-256: 98c79c9cc26b82d02262dded4c61de331d746fd8e32cf02bb3b4791937799190
