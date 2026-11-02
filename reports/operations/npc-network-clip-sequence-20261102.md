## 1. 작업 목표

정지 Run에서 관찰한 실제8파트 network mode2 후보를 동작 전환·작은 위치 변경·사망 뒤 수명 정리까지 확장한다. NPC1명의8파트이며8NPC 시험이 아니다. 현재 정상 AI/접촉/낙하 파이프라인을 바로 이식하지 않고 채택 선행조건을 검증한다. 이전 골 턴은 기존 결과 대조·무결성 검증·공개 보고로 authoritative state를 갱신한 progress였다.

## 2. 실제 수행

기존5개 서명 PBO·공식 진단 server/client·8개 물리 파트·동일NPC 내부28쌍 차단·기존14클립 native graph를 재사용했다. 새 PBO·그래프·클립·보간·native 숫자 덮어쓰기는 없다. 네이티브 관측기를 연결하지 않았다.

별도 mission에서 기존 DGBNativeGroundBody를 애니메이션 요청 저장용으로만 사용했다. 모든 파트 GroundLeader=false이므로 Drive는 실행하지 않는다. Request/Released/Landed를 정해진 순서로 제공하고 Run 구간만 전체8파트에 동일한X축±0.35m 위치를 지정했다. 이는 실제 경로 추적·충돌·등반 이동을 검증한 것이 아니다. 등반은 마지막 포즈를 의도적으로 기다렸다가 해제한다. 두 사망은 부활시키지 않고,8파트 BeginRetirement→스크립트 명령 해제 확인→보통 ObjectDelete→새8파트 생성으로 구분했다.

OBS 준비 뒤 Computer Use로 실제 게임 전경과 고정 시점을 확인하고 시작 gate를 열었다. 게임 창만20초640×640·60fps·무음 lossless로 녹화했다. 녹화 후 OBS 설정 원문을 복구하고 프로세스를 닫았다.7.4MB MP4는 사용자 검토용 손실 압축 미리보기이며 시간 재조정/중간 프레임 생성 없이 원본을 보존한다. 원시 영상은 공개 GitHub에 올리지 않는다.

## 3. 측정 결과

|항목|실제 결과|범위|
|---|---|---|
|논리 개체/파트|동시1NPC·8파트,교체2세대|독립2회 반복이 아님|
|상태 요청|0~13 모두 각8파트 NativeRequest에서 확인|그래프 상태 이름 직접 계측 아님|
|CPU 표본|4,384행·완전한548개8파트 묶음|약0.1초 희소 표본|
|기준 위치 최대 파트 간 거리|기록 정밀도상0m|직접 동일 위치를 지정한 통제 조건|
|실제 root X 범위|4549.650879~4550.349609|원점4550 기준 약±0.35m|
|공통5본|Head/Hips/spine2/Neck/LeftHand 각548묶음 최대 차이0|RightHand는8파트 공통 본 조건 미충족·별도 성공으로 세지 않음|
|포즈 진행|24개 요청/해제 구간 모두 Head MS 변화 존재|전체14클립 화면 품질 입증 아님|
|퇴역/삭제|2세대 모두8명령 해제 확인 후 삭제|서버/클라이언트 최종 정상 종료 계약과 별개|
|캡처 전경|1,025/1,025 trace active|거친 시계 연결·±0.25초 padding|
|mission dt p50/p95/p99|20/23/23ms|Present·CPU busy·표시 포즈 간격 아님|
|녹화|1,202/1,202출력 프레임 barcode 연결·중복 marker4|게임/본60fps를 뜻하지 않음|
|원시 영상|75,532,738bytes·20초|성능 시험 제외|

클라이언트 exit=-1·강제 종료false,서버 DONE 뒤 harness 정리로 exit=-1·강제 종료true. 이전 network 관측 실행의0xc0000005 종료 코드가 이번에 관찰되지는 않았지만 정상 종료 인증이나 관측기 원인 확정으로 해석하지 않는다. 이번에 Present/CPU busy/GPU p50/p95/p99·할당·GC·64개체 성능은 측정하지 않았다.

climb의 Head 변화는53쌍 중20,climb_low는44쌍 중13이었다. 나머지를 전부 프레임 드롭으로 세지 않는다. 이 fixture는 유한 클립 끝 포즈를 release 전까지 유지하도록 설계됐다. near-still 이미지205/1201쌍 역시 의도된 hold·캡처 중복·배경/TAA를 포함하므로 끊김 수치가 아니다. 기존 Run 전용 영상과 이 복합 동작 영상의 분포를 직접 빼서 개선률을 만들지 않는다.

## 4. 원인 분석

확인:8파트 network 경로는 정지 Run만 가능한 상태를 넘어 요청 전환·작은 root 위치 변경·명령 해제·재생성까지 이어졌다. 공통 CPU 본과 root가 표본 시점에 일치했고 모든 요청 구간에서 포즈 변화가 있었다. client 화면 선택6장에서도 몸체와 서로 다른 포즈가 보였다. 정지 이미지로 시간적 부드러움을 판정하지 않는다.

준비 실패: mode2-5는 기존 알려진 PluginConfigDebugProfile 오류의 CreateCustomMission 줄 번호가113에서210으로 바뀌어 정확 일치 guard가 거부했다. 알려진8줄의 경로·줄 번호만 반영한 mode2-6으로 실행했다. SCRIPT(E) 전체 허용이나 임의 오류 무시는 추가하지 않았다. 실패 profile·강제 정리·녹화 미시작을 보존하며 기능 반복으로 세지 않는다.

가설: native network buffer/표시 경로는 mode0의 시간 손실·계단형 출력 개선 후보로 남는다. 이번 시험은 서버 CPU 포즈 연결과 실제20초 창 녹화가 함께 있지만, CPU bone↔최종 render pose의 프레임별 동등성·정상 속도·주관적 부드러움은 최종 확인하지 못했다. root를 동일 위치로 강제한 결과를 실제 외부 접촉 안정성으로 일반화하지 않는다. 추가7물리 host·복제·내부28block의 비용을 아직 모르므로 최적화 후보로 수용하지 않는다.

채택 판단: CONTINUE_ISOLATED / 제품 적용 보류. 연결 경로 자체가 실패했다는 이유로 폐기할 단계는 아니지만 현재 정상 파이프라인 교체·64개체 확대를 허용할 증거도 부족하다. 비용 연구와 시각 품질 연구를 섞어 성공 판정을 만들지 않는다.

## 5. 검증 수준

요청14종 전달·공통 CPU 본/기준 위치 일치·퇴역2세대: 단일 시나리오 진단 성공. 실제20초 전경 창 녹화·표본 이미지 확인: 수집/연결 진단 성공.14클립 최종 화면 품질·정상 전투/피격·외부 밀침/낙하/재등반·8/64 기능 회귀·반복 비용 개선·20ms p95·최종 수용: 미완료다. 전체 골은active다.

## 6. 변경 사항

visible-match 아래 mode2-5/6 격리 mission·기존 런처 사본·분석·녹화 파생본만 추가했다. 원본 source/PBO·운영 서버·Steam·ANM/XOB/ASI/그래프·시계/LOD 구현은 보존했다. 기존5PBO 해시와 서명을 실행 전 재검증했다. 사용한 parts PBO8BDA6B7A8C95962EF0FDA6BBC6AA228906504C3130D4597070ED2E1E53F7DF9C, camera PBO87442808975539355CB720F88927FF69FF451A92C2DF49394A4F70C89C0247B4. 새PBO가 없으므로 새 서명 대상은 없다. 개인키는 공개하지 않는다.

OBS 원래 basic/scene 해시는 각각32eec73e6a4d4f468f823669eae348806c304bd755ac4137361a163b70af3a92 /17a9ca4f7cec5d62409f90ea8687e48b8b99c9f474dbc75a46f017d010609cb9이며 원문 복구를 확인했다. 런처의 일반 newVideo=false 표식과 달리 이 묶음에는 실제 녹화와 미리보기가 생겼음을 명시한다.

## 7. 원시 증거

sourceRunId: animation-network-role-20261101-1600/eight-part-20261102/visible-match/mode2-6. sequence-events/role-pose/script/RPT,두 launch,plan,sequence-analysis,sequence-pose-motion,recorder-result,markers,focus-final,visible-mask,preview-derivation와 원시 영화 SHA를 로컬 무결성 목록으로 연결한다. capture-focus-analysis는 실험 중 생성됐고,종료 후 읽은capture-focus-analysis-final로 최종 판정했다. 전경·dt 결과는 동일했다. source mode2-5 setup 실패와 원문도 연결한다. 같은 기록의 분석을 새 독립 실행으로 세지 않는다.

원시 MKV SHA-256 d953b123dd911cb0c74f62188d4410a47fa6aa53309bb0e69c7063be12cdd493. 미리보기 SHA-256 d11b47f161c4610e6a597dc24e5a7d2a277e8a44f52278fea90788330e07009e. 공개 packet은 자체 제어 함수 발췌와 집계이며 전체 로그/영상/게임 자산/로컬 경로/프로세스 정보는 제공하지 않는다.

## 8. 잔여 문제

다음 구현 선행조건은 현재 실제 GroundBody가 소유하는 이동·외부 접촉·피격/낙하를 server-authoritative network 표시 후보에 연결할 때의 최소 변경 범위다. 현재 원본 Initialize의 생성 flag·파트 type·기준 위치 commit·retirement 소유권을 대조해 한 NPC부터 보존 가능한지 확인한다. 새 군집 알고리즘이나 그래프를 만들지 않는다. 구조상 추가 host 비용이 크게 늘거나 외부 접촉을 보존할 수 없으면 이 후보를 제품 경로로 채택하지 않는다.

시각은 사용자 검토 가능한20초 미리보기를 제공한다. 일시정지/끝 포즈 hold를 끊김으로 혼동하지 않고 idle/walk/run과 이동 구간의 재생을 우선 평가한다.14클립 전체 화면 수용은 별도다. 물리 scratch 반복 성능과 SupportFast 안전성은 기존 독립 실험으로 보존한다. 기능 통과만으로64성능 개선을 주장하지 않는다.

로컬 checkpoint SHA-256: dbcd5cdcd79d153045cfb0eda255af83c8a514a8482aa4f1e9ff01191e1d6351
