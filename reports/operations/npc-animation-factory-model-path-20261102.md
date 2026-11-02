## 1. 작업 목표

네트워크8파트 애니메이션 시험이 평가 전에 생성 단계에서 실패하는 이유를 좁힌다. 기존 기능·자산·운영 PBO는 보존하고 실제 모델 입력을 확인한 뒤 최소 경로 후보만 시험한다. 이 작업은 평균 배속·화면 끊김·64성능의 해결 판정이 아니다.

## 2. 실제 수행

초기8파트 서버 생성과 단일 파트 통제를 조사했다. Geometry LOD 추가 후보, HeadLook 기준점을head에서hips로 바꾸는 후보는 생성 충돌을 피하지 못했다. 이때 Geometry 후보 실패를 곧바로 Geometry 효과 부재로 분류했던 해석을 이번 증거로 정정한다. 해당 후보 P3D가 열리지 않았기 때문이다.

정확한 A500 실행 파일 해시와 로그 명령 바이트를 대조했다. 해당 바이트열은 한 곳이며, 기록된 모듈 베이스8건과대조한 fault RVA는0xc68039다. PE unwind 범위0xc67fa0~0xc68096를 Capstone으로 전부 디코드했다. entity+0x1a8에서 선택 자료를 찾고 byte+0x1ce가 음수이거나 slot이 비면0참조를 만든 뒤+0x54를 읽는 경로다. 별도0x958b10 시작·chained 범위에서는 약1e13 해상도를 식별해+0x1ce에 저장하는 정적 근거를 확인했다. 같은 오프셋의 다른 구조체 참조를 섞어 타입을 확정하지 않는다.

한 번의 읽기 전용 관측은 바이트 가드 길이 오류로 실패했다. 원시 오류를 보존하고 가드만 고친 별도 실행에서 실제 입력을 읽었다. 메모리 수치·시계·mode를 쓰지 않았으며 entry hook은 사용했다. 이 계측 비용을 성능 비교에 사용하지 않는다.

P3D 내부 모델·골격·ANM은 그대로 두고, 별도 설정 PBO에서 model 경로 구분자만 /에서 역슬래시로 바꾼 하위 클래스를 만들었다. 기존 Geometry 후보의 동일 packed model을 참조한다. 자동 서명·서명 검증·packed script 일치를 확인한 뒤 같은 headless factory 임무와 관측으로 실행했다. 원래 배포 PBO와 Steam·운영 서버는 변경하지 않았다.

## 3. 측정 결과

|항목|구분자 수정 전 Geometry 후보|경로 수정 후보|
|---|---|---|
|실제 ConfigGetText 모델|nf_net_phys/nf_part1_geometry.p3d|nf_net_phys 역슬래시 nf_part1_geometry.p3d|
|native slot count / Geometry index|0 / -1|4 /3|
|선택 Geometry body|찾지 못함|참조 존재|
|CreateObjectEx 반환 / CREATED|미도달|확인|
|factory 이후 명령 시험10초|미도달|DONE 확인|
|마지막 표본 nativeSeconds /updates|없음|9.6 /48|

NativeSeconds는 스크립트 command 시간 집계이며 graph clock 또는 화면 재생시간 측정이 아니다. server 약200ms 갱신 한계가 해결됐다는 근거도 아니다. corrected-path 성공은 단일 실행이다.

기록된 runtime factory 시도는 총8건이며 초기8파트 시도1·단일 파트 시도7이다. 앞선 compile 준비 실패2건과 포트 preflight 거부는 factory 실행으로 세지 않는다. 첫 native 관측 실패1건은 실행 자체와 계측 유효성을 분리한다. initial network client는8파트 READY에 도달하지 못했고 원래8파트 observer는 attach 전에 종료됐다.

8/64 gameplay 회귀·Present/CPU/GPU p50/p95/p99·할당/GC·실제 표시 cadence는 이번 미측정이다. 정상 밀침·낙하·착지·FOLLOW·재등반의 새 회귀 통과도 주장하지 않는다.

## 4. 원인 분석

확인: / 경로 후보는 RPT에 Cannot open object를 남겼고 실제 LOD가0이었다. 같은 packed Geometry model과 native animation 설정을 유지하면서 model 경로를 역슬래시로 바꾼 alias에서 LOD4·Geometry3이 나타나고 생성과 명령 시험이 완료됐다. 최소한 이 후보의 파일 로딩 실패는 경로 표기와 연결된다. 모든 DayZ 리소스 API가 /를 거부한다는 일반화는 하지 않는다.

확인: packed Geometry 후보는ODOL56·LOD4이고원본동일 control은LOD3이다. 기존 source 모델 LOD byte보존 proof와 packed header/native 실측을 구분한다. packed visual·전체 골격 의미 동등성을 이 헤더만으로 인증하지 않는다.

정정: 앞선 Geometry 단독 변경 실패는 모델 미로딩이 섞인 무효 효능시험이었다. Geometry 자체가 불필요하다는 반증이 아니다. head→hips 변경만으로 생성이 해결되지 않았다는 관찰은 유지하지만, 이 실패의 직접 원인을 head 이름으로 특정하지 않는다. 원래7개 렌더 파트를 각각 물리 host로 만들 때 필요한 모델 계약과 비용은 여전히 미확인이다.

가설: 원래 렌더 파트의 Geometry 부재도 초기 동일 null 경로에 기여할 수 있다. 나머지 실제 파트의 로딩/선택 결과를 확인하기 전8파트 해법으로 선언하지 않는다. 네이티브 null 검사를 패치하거나 geometry index를 강제로 쓰지 않는다.

## 5. 검증 수준

모델 경로와 충돌 입력 연결: 진단 성공. 단일 파트 factory: 한 실행 범위 통과. 반복 검증 완료·8파트 네트워크 동기화·14클립·8/64 기능 회귀·최종 시각/성능 수용은 미완료다.20ms p95는 계속 목표다. Ghidra의 별도 focused 요청은 초기화 중이며 완료 디컴파일을 확보한 것처럼 보고하지 않는다.

## 6. 변경 사항

실험 root 안에 source MLOD LOD 목록·경계 mapper·native read observer·격리 factory 임무/런처와 진단3PBO를 추가했다. Geometry33AEF25A..., Headpoint9D3F9DCC..., path E73A9CBB...는 각각 서명·검증 성공이다. path의 전체 buildId는 E73A9CBB902850F0883120C0F7A18626643A5B4C9703A99BA04D0A407EDE0877이다. 개인키는 배포/공개에 포함하지 않았다. Geometry build의 generic newGameplayAlgorithms=true 표시는 템플릿 표식이며 이 묶음에서 gameplay 알고리즘을 변경했다는 증거로 사용하지 않는다.

원본6PBO/기존 자산·Source·현재 배포·운영 서버·기존 보고서는 보존했다. 최초 guard 오류를 덮어쓰지 않았으며 기존 관측기의 가드 수정 및 별도 실행을 명시한다. 이 report 이후8파트 자산 변경은 아직 없다.

## 7. 원시 증거

sourceRunId: animation-network-role-20261101-1600/eight-part-20261102/factory-model-path. 로컬 review/batch-results.json·module-base-crosscheck·packed-model-header-audit, factory-fault-static.json, shape-field-static.json, source-model-lods/source-memory-comparison, physical-part1/source-proof 및 세 build/signature 증거를 연결한다. 각 factory의 launch·stage TSV·RPT/crash·관측 JSON과 corrected-path pose TSV를 보존한다. 초기 compile 실패·0바이트 minidump·포트 거부도 남기며 유효 dump 분석으로 세지 않는다.

게임/표시 계측 프로세스가 없고 모든 trial launch가 terminal임을 확인했다. headless harness는 DONE 뒤에도 자신이 띄운 프로세스를 강제 정리하므로 정상 종료를 주장하지 않는다. 진행 중인 Ghidra 초기화는 이 게임 실험과 별개이며 중단하지 않는다. 공개 자료는 집계·선택 입력·상대 RVA만 담고 원시 자산·절대 경로·프로세스 정보·개인키를 제외한다.

## 8. 잔여 문제

다음은 나머지 실제 파트의 packed 모델 로딩·Geometry 선택을 확인해8파트 시험의 생성 선행조건을 통과시키는 것이다. 고정 경로 후보는 진단용으로 보존하며 모든 파트를 물리 host로 바꾸는 설계를 제품에 즉시 채택하지 않는다. 이후 client mode2 cohort·파트 간 포즈/시간·실제 표시·물리 host 비용을 각각 판정해야 한다. 물리 query scratch 후보의 반복 성능·착지 복구 반복은 별도 하네스/결과로 유지한다. 이번 factory 성공으로 애니메이션 끊김 해결이나 비용 절감을 보고하지 않는다.

로컬 checkpoint SHA-256: 831458de4e266952d7f4d2fbd5c3501dbf27e94d218b324e6dd910599ad3dc6a
